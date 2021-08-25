#include <Arduino.h>
#include <ChibiOS_ARM.h>
#include "CANBus.h"
#include "circular_buffer.h"
#include <Ethernet2.h>

#define PACKET_LENGTH 34
#define ACCELERATION_IDX 2
#define POSITION_IDX 6
#define VELOCITY_IDX 10
#define BATTERY_VOLTAGE_IDX 14
#define N_MODULES 4
//idx stands for packet index

//flexcan config script made with help of
//http://eet.etec.wwu.edu/KurtTaylor/project/docs/Bibleography/MCU/ConnSoftDocs/Kinetis%20SDK%20v2.0.0%20API%20Reference%20Manual/group__flexcan__driver.html
flexcan_config_t flexcanConfig;

FLEXCAN_GetDefaultConfig(&flexcanConfig);
FLEXCAN_Init(CAN_SETUP, &flexcanConfig);
FLEXCAN_Enable(CAN_SETUP, true);

FlexCAN Cbus(500000);
CircularBuffer<CAN_message_t, 10> in_buff, out_buff;
uint16_t wait_time = 40; // 40 ms
uint8_t CAN_id = 0;

EthernetUDP Udp;

uint32_t tube_length = 125000;
double run_length = 30.0;
uint8_t team_id = 0;

double seconds = 0;
unsigned long wait_time = 400; // 400 ms

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress local_ip(192, 168, 2, 177);
IPAddress remote_ip(192, 168, 2, 30);

unsigned int local_port = 3000;
unsigned int remote_port = 3000;

static struct States
{
    uint32_t pod_state = Status::SafeToApproach;
    bool ready_to_send = false;
    bool fault_detected = false;
    uint32_t fault_type = FaultType::No_Fault;
    double position = 0;
    double velocity = 0;
    double acceleration = 0;
    bool should_launch = true;
} pod_context;

enum Status
{
    Fault = 0,
    SafeToApproach = 1,
    ReadyToLaunch = 2,
    Launching = 3,
    Coasting = 4,
    Braking = 5,
    Crawling = 6
};

enum Msg_Type
{
    Telemetry = 0,
    Command = 1,
    Fault = 2
};

enum CANID_List
{
    General = 0,
    BMS = 1,
    Nav = 2,
    Brk = 3,
    Mtr = 4
};

enum FaultType
{
    No_Fault = 0,
    Gen_Fault = 1,
    Mtr_Fault = 2,
    Nav_Fault = 3,
    Brk_Fault = 4
};

// TODO: implement this function
uint8_t get_status()
{
    return 0;
}


//to-do: fit canbus messages here
double get_position(double seconds, double run_length, uint32_t tube_length)
{
    return (1 - 1 * cos((double)seconds * PI / (double)run_length)) / 2.0 * tube_length;
}

double get_velocity(double seconds, double run_length, uint32_t tube_length)
{
    return (get_position(seconds, run_length, tube_length) - get_position(seconds - 0.1, run_length, tube_length)) * 10;
}

double get_acceleration(double seconds, double run_length, uint32_t tube_length)
{
    return (get_velocity(seconds, run_length, tube_length) - get_velocity(seconds - 0.1, run_length, tube_length)) * 10;
}

uint8_t *build_packet(uint8_t team_id, uint8_t status, int32_t acceleration, int32_t position, int32_t velocity)
{
    static uint8_t packet[PACKET_LENGTH];
    packet[0] = team_id;
    packet[1] = status;

    for (int i = ACCELERATION_IDX + 3; i >= ACCELERATION_IDX; i--)
    {
        packet[i] = acceleration & 0xFF;
        acceleration = acceleration >> 8;
    }

    for (int i = POSITION_IDX + 3; i >= POSITION_IDX; i--)
    {
        packet[i] = position & 0xFF;
        position = position >> 8;
    }

    for (int i = VELOCITY_IDX + 3; i >= VELOCITY_IDX; i--)
    {
        packet[i] = velocity & 0xFF;
        velocity = velocity >> 8;
    }

    for (int i = BATTERY_VOLTAGE_IDX; i < PACKET_LENGTH; i++)
    {
        packet[i] = 0;
    }

    return packet;
}

void print_packet(uint8_t *packet)
{
    for (int i = 0; i < PACKET_LENGTH; i++)
    {
        Serial.print(packet[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

uint8_t *fault_msg(uint8_t CAN_id, uint8_t fault_type)
{
    if (fault_detected)
    {
        switch (fault_type)
        {
        case FaultType::Gen_Fault:
            /* insert response */
            break;

        case FaultType::Mtr_Fault:
            /* insert response */
            break;

        case FaultType::Nav_Fault:
            /* insert response */
            break;

        case FaultType::Brk_Fault:
            /* insert response */
            break;

        default:
            /* insert response */
            break;
        }
    }
    else
    {
        return -1; //error, fault hasn't been identified
    }

    return 0;
}

CAN_message_t *build_msg(uint8_t recipient_id, uint8_t msg_type)
{
    CAN_message_t out_msg;
    out_msg.ext = 0;
    out_msg.id = recipient_id;
    out_msg.len = PACKET_LENGTH;
    for (size_t i = 0; i < PACKET_LENGTH; i++)
    {
        out_msg.buf[i] = 0x0F; //just a placeholder. fill according to situation.
    }

    // out_msg.timeout = wait_time;

    //cbus.write(out_msg);
    return out_msg;
}

THD_WORKING_AREA(waThread1, 128);

static THD_FUNCTION(Thread1, arg)
{
    (void)arg;
    chRegSetThreadName("canbus_task");
    while (1)
    {
        //add getter for ready to send flag here
        if (pod_context.ready_to_send)
        {
            //read context file to know request type here
            msg_type = Msg_Type::Telemetry;     //placeholder
            recipient_id = CANID_List::General; //placeholder
            CAN_message_t *out_msg = build_msg(recipient_id, msg_type);
            out_buff.Write(out_msg);
            //Cbus.write(out_msg);
        }

        //begin reading messages
        while (Cbus.available())
        {
            CAN_message_t in_msg;
            Cbus.read(in_msg);
            in_buff.Write(in_msg);
        }

        //unpacking messages
        for (size_t i = 0; i < in_buff.numElements(); i++)
        {
            CAN_message_t in_msg = in_buff.Read();

            //here, check the contents of the message
            msg_type == Msg_Type::Telemetry //placeholder. change.
            if (msg_type == Msg_Type::Telemetry)
            {
                //handle each CANbus member differently
                //add to context, and pass info to UDP module via context
            }
            elif (msg_type == Msg_Type::Fault)
            {
                //handle each CANbus member differently
                recipient_id = CANID_List::General;
                CAN_message_t *out_msg = build_msg(0, msg_type);
                out_buff.Write(out_msg);
            }
        }
        delay(wait_time);
        chThdYield();
    }
}

THD_WORKING_AREA(waThread2, 128);

static THD_FUNCTION(Thread2, arg)
{
    (void)arg;
    chRegSetThreadName("telemetry_module");
    while (1)
    {
        unsigned long start_time = millis();
        uint32_t status = pod_context.pod_state;
        if (status == Status::SafeToApproach && should_launch)
        {
            if (seconds > 10)
            {
                status = Status::ReadyToLaunch;
                seconds = 0;
                should_launch = false;
            }
        }
        else if (status == Status::ReadyToLaunch)
        {
            if (seconds > 5)
            {
                status = Status::Launching;
                seconds = 0;
            }
        }
        else if (status == Status::Launching)
        {
            position = get_position(seconds, run_length, tube_length);
            velocity = get_velocity(seconds, run_length, tube_length);
            acceleration = get_acceleration(seconds, run_length, tube_length);

            if (acceleration < 0)
            {
                status = Status::Braking;
            }
        }
        else if (status == Status::Braking)
        {
            position = get_position(seconds, run_length, tube_length);
            velocity = get_velocity(seconds, run_length, tube_length);
            acceleration = get_acceleration(seconds, run_length, tube_length);

            if (seconds >= run_length)
            {
                status = Status::SafeToApproach;
            }
        }
        else if (status == Status::SafeToApproach)
        {
            position = tube_length;
            velocity = 0;
            acceleration = 0;
        }

        uint8_t *packet = build_packet(team_id, status, (int32_t)acceleration, (int32_t)position, (int32_t)velocity);
        print_packet(packet);
        Udp.beginPacket(remote_ip, remote_port);
        Udp.write(packet, (size_t)PACKET_LENGTH);
        Udp.endPacket();

        unsigned long end_time = millis();
        delay(wait_time - (end_time - start_time));
        seconds += ((double)wait_time / (double)1000);


        chThdYield();
    }
}

void setup()
{
    Cbus.begin();
    halInit();
    chSysInit();
    chThdCreateStatic(waThread1, sizeof(waThread1),
                      NORMALPRIO, Thread1, NULL);
    chThdCreateStatic(waThread2, sizeof(waThread2),
                      NORMALPRIO, Thread2, NULL);
}

void loop()
{
    Serial.println("Main thread");
    chThdYield();
}