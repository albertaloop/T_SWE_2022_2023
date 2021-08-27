#include <Arduino.h>
#include <ChibiOS_ARM.h>
#include "CANBus.h"
#include "circular_buffer.h"

//flexcan config script made with help of
//http://eet.etec.wwu.edu/KurtTaylor/project/docs/Bibleography/MCU/ConnSoftDocs/Kinetis%20SDK%20v2.0.0%20API%20Reference%20Manual/group__flexcan__driver.html
flexcan_config_t flexcanConfig;

FLEXCAN_GetDefaultConfig(&flexcanConfig);
FLEXCAN_Init(CAN_SETUP, &flexcanConfig);
FLEXCAN_Enable(CAN_SETUP, true);

FlexCAN Cbus(500000);
uint16_t wait_time = 40; // 40 ms
CircularBuffer<CAN_message_t, 10> in_buff, out_buff;
uint8_t CAN_id = 4;
uint8_t msg_type = Msg_Type::SafeToApproach;

bool movement_request = false;
bool message_read = false;
uint8_t movement_state = MovementState::SafeToApproach;

double seconds = 0;
unsigned long wait_time = 400; // 400 ms


enum MovementState
{
    Still = 0,
    Accelerating = 1,
    Decelerating = 2,
    Cruising = 3
};

enum Msg_Type
{
    None = 0,
    Telemetry = 1,
    Accelerate = 2,
    Decelerate = 3
};

static struct States
{
    uint8_t movement_state = Status::Still;
    double speed = 0;
    double desired_speed = 0;
    double desired_accel = 0;
    bool ready_to_send = false;
    bool fault_detected = false;
    uint32_t fault_type = FaultType::No_Fault;
    uint8_t msg_type = Msg_Type::None;
} pod_context;

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

uint8_t get_state(struct States *context)
{
    return context->movement_state;
}

void set_state(struct States *context, uint8_t state)
{
    context->movement_state = state;
}

bool is_fault(struct States *context)
{
    return context->fault_detected;
}

uint8_t get_msg_type(struct States *context)
{
    return context->msg_type;
}

void set_msg_type(struct States *context, uint8_t msg_type)
{
    context->msg_type = msg_type;
}

//to-do: fit canbus messages here
double get_position()
{
    return 0;
}

double get_speed()
{
    return context->speed;
}

double get_acceleration()
{
    return 0;
}

double get_desired_speed(struct States *context)
{
    return context->desired_speed;
}

double get_desired_accel(struct States *context)
{
    return context->desired_accel;
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
    chRegSetThreadName("communication_task");
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
    chRegSetThreadName("temp_monitor_task");
    while (1)
    {
        //add task code here
        chThdYield();
    }
}

THD_WORKING_AREA(waThread3, 128);

static THD_FUNCTION(Thread3, arg)
{
    (void)arg;
    chRegSetThreadName("movement_task");
    while (1)
    {
        //start by checking if any requests have been made
        if (movement_request)
        {
            bool fault_detected = is_fault(&pod_context);
            if(fault_detected) {
                //stop motor
            }
            uint8_t msg_type = get_msg_type(&pod_context);
            message_read = true;
            set_msg_type(&pod_context, Msg_Type::None);

            switch (msg_type)
            {
            case Msg_Type::Telemetry:
                /* code */
                break;

            case Msg_Type::Accelerate:
                double desired_speed = get_desired_speed(&pod_context);
                double desired_accel = get_desired_accel(&pod_context);
                double speed = get_speed(&pod_context);

                if (desired_speed>speed)
                {
                    set_state(&pod_context, MovementState::Cruising)
                }
                

                break;

            case Msg_Type::Decelerate:
                /* code */
                break;

            default:
                break;
            }
        }
        

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
    chThdCreateStatic(waThread3, sizeof(waThread3),
                      NORMALPRIO, Thread2, NULL);
}

void loop()
{
    Serial.println("Main thread");
    chThdYield();
}