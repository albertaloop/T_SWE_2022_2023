#ifndef _STMCHN_H_
#define _STMCHN_H_

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FlexCAN_T4.h>

#include "ChRt.h"
#include "BamocarInterface.h"
#include "MessageMaster.h"

enum MovementState
{
  Still = 0,
  Accelerating = 1,
  // note: accelerating can mean deceleration as well
  // we are combining accel and decel to reduce chances of errors
  Cruising = 2,
};

enum FaultType
{
  No_Fault = 0,
  Gen_Fault = 1,
  Mtr_Fault = 2,
  Nav_Fault = 3,
  Brk_Fault = 4
};

/*
  State Machine class code.
  Contains the actual 'states' and behaviour
  of the State Machine.

  Comment: The reason this is in snake_case while
  everything else I wrote isn't, is because
  I used to really like snake_case, but eventually
  started to like camelCase more after learning Java.
    --Fiaz

  */
class StateMachine {
  private:
    MessageMaster *msgMaster;
    BamocarInterface *bamocar;

    FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;
    FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;

    uint8_t movement_state = MovementState::Still;
    int position[3] = {0, 0, 0};
    float velocity[3] = {0, 0, 0};
    float accel[3] = {0, 0, 0};

    uint32_t distance_travelled = 0;
    uint8_t battery_level = 100;
    float speed = 0;
    float accel_scalar = 0;
    float desired_speed = 0;
    float desired_accel = 0;
    bool telemetry_request = false;
    bool fault_detected = false;
    bool ready_to_send_canbus = false;
    bool routine_just_changed = false; // flag to see if routine was JUST changed


    uint8_t fault_type = FaultType::No_Fault;
    uint8_t msg_type = CanMessages::Start;

  public:
    const uint8_t max_dist_travelled = 20; // in meters
    const uint8_t CAN_id = 4;
    StateMachine(MessageMaster &msgMaster, BamocarInterface &bamocar);

    uint8_t get_state();
    bool is_fault();
    bool is_just_changed();
    uint8_t get_msg_type();
    void get_position(int position[3]);
    float get_speed();
    void get_velocity(int velocity[3]);
    float get_acceleration_scalar();
    void get_acceleration(int accel[3]);
    float get_desired_speed();
    float get_desired_accel();
    bool get_ready_to_send_canbus();
    uint32_t get_distance_travelled();
    uint8_t get_battery_level();

    void set_battery_level(uint8_t percentage);
    void set_distance_travelled(uint32_t meters); // in meters
    void set_state(uint8_t state);
    void set_just_changed(bool just_changed);
    void set_msg_type(uint8_t msg_type);
    void set_speed(float speed);
    void set_position(int position[3]);
    void set_velocity(int velocity[3]);
    void set_acceleration(int accel[3]);
    void set_acceleration_scalar(float accel);
    void set_ready_to_send_canbus(bool flag);
    void set_fault(bool flag);

    bool fault_msg(uint8_t fault_type);

    void read_msg();
    void msg_tasks();
    void movement_tasks();
};


#endif