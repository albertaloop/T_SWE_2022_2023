#ifndef _UDP_H_
#define _UDP_H_
#include <FlexCAN_T4.h>
#define PACKET_LENGTH 34
#define ACCELERATION_IDX 2
#define POSITION_IDX 6
#define VELOCITY_IDX 10
#define BATTERY_VOLTAGE_IDX 14


uint8_t *build_packet(uint8_t team_id, uint8_t status, int32_t acceleration, int32_t position, int32_t velocity);
enum Status {
    Fault = 0,
    SafeToApproach = 1,
    ReadyToLaunch = 2,
    Launching = 3,
    Coasting = 4,
    Braking = 5,
    Crawling = 6
};

struct {
    CAN_message_t nav_recenter; // Nav module to set origin to current position.
    CAN_message_t state_estop; // Brake controller to fully apply brakes, Motor Controller shut down.
    CAN_message_t state_prepare_launch; // Brake controller to test brakes, and Motor Controller
                                        // to start up.
    CAN_message_t state_begin_launch;   // Brake controller realase brakes, Motor Controller begin
                                        // propulsion.
    CAN_message_t state_soft_stop;      // Brake controller apply brakes, Motor Controller stop
                                        // propulsion.
    CAN_message_t state_shut_down;      // 

} CAN_messages;

void init_CAN_messages();
#endif