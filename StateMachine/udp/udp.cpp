#include <Ethernet2.h>

#define PACKET_LENGTH 34
#define ACCELERATION_IDX 2
#define POSITION_IDX 6
#define VELOCITY_IDX 10
#define BATTERY_VOLTAGE_IDX 14

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress local_ip(192, 168, 2, 177);
IPAddress remote_ip(192, 168, 2, 30);

unsigned int local_port = 3000;
unsigned int remote_port = 3000;

EthernetUDP Udp;

uint32_t tube_length = 125000;
double run_length = 30.0;

uint8_t team_id = 0;

double seconds = 0;

unsigned long wait_time = 400; // 400 ms

bool should_launch = true;

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

uint8_t status = Status::SafeToApproach;

double position = 0;
double velocity = 0;
double acceleration = 0;

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

void setup()
{
    Ethernet.begin(mac, local_ip);
    Udp.begin(local_port);
    Serial.begin(9600);
}

void loop()
{
    unsigned long start_time = millis();
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
}