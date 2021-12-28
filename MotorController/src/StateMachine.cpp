#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FlexCAN_T4.h>

#include "ChRt.h"
#include "StateMachine.h"
#include "BamocarInterface.h"
#include "MessageMaster.h"

StateMachine::StateMachine(MessageMaster &msgMaster, BamocarInterface &bamocar)
{
    this->msgMaster = &msgMaster;
    this->bamocar = &bamocar;
}

uint8_t StateMachine::get_battery_level()
{
    return this->battery_level;
}

void StateMachine::set_battery_level(uint8_t percentage)
{
    this->battery_level = percentage;
}

uint32_t StateMachine::get_distance_travelled()
{
    return this->distance_travelled;
}

void StateMachine::set_distance_travelled(uint32_t meters)
{
    this->distance_travelled = meters;
}

uint8_t StateMachine::get_state()
{
    return this->movement_state;
}

void StateMachine::set_state(uint8_t state)
{
    this->movement_state = state;
}

void StateMachine::set_fault(bool flag)
{
    this->fault_detected = true;
}

bool StateMachine::is_fault()
{
    return this->fault_detected;
}

bool StateMachine::get_ready_to_send_canbus()
{
    return this->ready_to_send_canbus;
}

void StateMachine::set_ready_to_send_canbus(bool flag)
{
    this->ready_to_send_canbus = flag;
}

bool StateMachine::is_just_changed()
{
    return this->routine_just_changed;
}

void StateMachine::set_just_changed(bool just_changed)
{
    this->routine_just_changed = just_changed;
}

uint8_t StateMachine::get_msg_type()
{
    return this->msg_type;
}

void StateMachine::set_msg_type(uint8_t msg_type)
{
    this->msg_type = msg_type;
}

void StateMachine::get_position(int velocity[3])
{
    for (int8_t i = 0; i < 3; i++)
    {
        velocity[i] = this->velocity[i];
    }
}

float StateMachine::get_speed()
{
    return this->speed;
}

void StateMachine::set_speed(float speed)
{
    this->speed = speed;
}

void StateMachine::get_velocity(int velocity[3])
{
    for (int8_t i = 0; i < 3; i++)
    {
        velocity[i] = this->velocity[i];
    }
}

void StateMachine::set_velocity(int velocity[3])
{
    for (int8_t i = 0; i < 3; i++)
    {
        this->velocity[i] = velocity[i];
    }
}

void StateMachine::set_position(int position[3])
{
    for (int8_t i = 0; i < 3; i++)
    {
        this->position[i] = position[i];
    }
}

float StateMachine::get_acceleration_scalar()
{
    return sqrt(this->accel[0] * this->accel[0] + this->accel[1] * this->accel[1] + this->accel[2] * this->accel[2]);
}

void StateMachine::get_acceleration(int accel[3])
{
    for (int8_t i = 0; i < 3; i++)
    {
        accel[i] = this->accel[i];
    }
}

void StateMachine::set_acceleration(int accel[3])
{
    for (int8_t i = 0; i < 3; i++)
    {
        this->accel[i] = accel[i];
    }
}

void StateMachine::set_acceleration_scalar(float accel)
{
    this->accel_scalar = accel;
}

float StateMachine::get_desired_speed()
{
    return this->desired_speed;
}

float StateMachine::get_desired_accel()
{
    return this->desired_accel;
}

bool StateMachine::fault_msg(uint8_t fault_type)
{
    uint8_t CAN_id=this->CAN_id;
    if (this->is_fault())
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
        return false; // error, fault hasn't been identified
    }
    return true;
}

/*
    Message Handling Here.
    We send messages to handle each flag.
*/
void StateMachine::read_msg()
{
    CAN_message_t in_msg;
    Can0.read(in_msg);

    uint8_t id = in_msg.id;

    if (id == CanMessages::Position)
    {
        // response for position change here
        int position[3] = {0, 0, 0};

        for (int8_t i = 0; i < 3; i++)
        {
            position[i] = in_msg.buf[2 * i + 1];
            if (in_msg.buf[2 * i] == 1)
            {
                position[i] = -position[i];
            }
        }
        set_position(position);
        int distance_travelled = sqrt(position[0] * position[0] + position[1] * position[1] + position[2] * position[2]);
        set_distance_travelled(distance_travelled);
    }
    else if (id == CanMessages::Velocity)
    {
        // response for velocity change here
        int velocity[3] = {0, 0, 0};
        for (int8_t i = 0; i < 3; i++)
        {
            velocity[i] = in_msg.buf[2 * i + 1];
            if (in_msg.buf[2 * i] == 1)
            {
                velocity[i] = -velocity[i];
            }
        }
        float speed = sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);

        this->set_velocity(velocity);
        this->set_speed(speed);
    }
    else if (id == CanMessages::Acceleration)
    {
        // response for velocity change here
        int accel[3] = {0, 0, 0};
        for (int8_t i = 0; i < 3; i++)
        {
            accel[i] = in_msg.buf[2 * i + 3];
            if (in_msg.buf[2 * i + 3] == 1)
            {
                accel[i] = -accel[i];
            }
        }
        this->set_acceleration(accel);
        float accel_scalar = sqrt(accel[0] * accel[0] + accel[1] * accel[1] + accel[2] * accel[2]);
        set_acceleration_scalar(accel_scalar);
    }
    else if (id == CanMessages::ChargeLevelUpdate)
    {
        // update battery level
        uint8_t percentage = in_msg.buf[0];
        set_battery_level(percentage);
        if (percentage<10) {
            set_fault(true);
            // TO-DO: send fault message
            set_ready_to_send_canbus(true);
        }
    }
    else if (id == CanMessages::BrakeFailure) {
        set_fault(true);
        // TO-DO: send fault message
        set_ready_to_send_canbus(true);
    }
}


/*
    Message Creation Here.
    Here we check the state and write messages:
        STATEMENT 1: If ready for telemetry
        STATEMENT 2: Velocity Change
        STATEMENT 3: Acceleration Change
*/
void StateMachine::msg_tasks()
{
    if (!get_ready_to_send_canbus()) {
        return;
    }

    if (is_fault()) {
        msgMaster->sendTemplateMain(BrakeEnable);
        //TO-DO: run end sequences
    }

}


void StateMachine::movement_tasks()
{
    CAN_message_t out_msg;

    // start by checking if any faults
    bool fault_detected = is_fault();
    if (fault_detected)
    {
        // INSERT: stop motor
    }

    // this switch contains code on running the routines!
    uint8_t movement_state = get_state();
    switch (movement_state)
    {
        case MovementState::Accelerating:
            float desired_speed = get_desired_speed();
            float desired_accel = get_desired_accel();

            // TO-DO: we must also add bamocar messages
            float speed = get_speed();

            // checks if state needs to be changed to cruise
            // speed must be within 2ms^-1 within desired speed to activate
            // cruise
            if (speed < (desired_speed + 2) && speed > (desired_speed - 2))
            {
                set_state(MovementState::Cruising);
                // trigger message asking bamocar to cut accel
            }

            // accel/decel routine
            else if (desired_speed > speed)
            {
                // trigger message asking bamocar to accelerate
            }

            // decel routine
            else if (desired_speed < speed)
            {
                // trigger message asking bamocar to decelerate
            }
            break;

        case MovementState::Cruising:
            desired_speed = get_desired_speed();
            desired_accel = get_desired_accel();

            // TO-DO: we must also trigger flag for speed request from bamocar
            speed = get_speed();

            // checks if state needs to be changed to cruise
            // speed must be within 2ms^-1 within desired speed to activate
            // cruise
            if (speed < (desired_speed + 2) && speed > (desired_speed - 2))
            {
                set_state(MovementState::Cruising);
            }

            // slight const accel if speed dropping is here
            if (desired_speed > speed)
            {
                // trigger message asking bamocar to accelerate
            }

            // decel routine if speed is too high here
            else if (desired_speed < speed)
            {
                // trigger message asking bamocar to decelerate
            }

            break;

        default:
            break;
    }
}

