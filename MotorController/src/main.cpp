#include <Arduino.h>
#include "MessageMaster.h"
#include <Wire.h>
#include <SPI.h>
#include <FlexCAN_T4.h>

#include <ChibiOS_ARM.h>
#include "circular_buffer.h"

FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;

uint16_t wait_time = 40; // 40 ms
CircularBuffer<CAN_message_t, 10> in_buff, out_buff;
uint8_t CAN_id = 4;
uint8_t msg_type = Msg_Type::SafeToApproach;

MessageMaster messageCreator;
uint8_t rcv_msg_list[3][5];
uint8_t snd_msg_list[9][4];
uint8_t nav_msg_list[3][2];

bool movement_request = false;
bool message_read = false;
uint8_t movement_state = MovementState::SafeToApproach;

double seconds = 0;
unsigned long wait_time = 400; // 400 ms

enum MovementState
{
  Still = 0,
  Accelerating = 1,
  // note: accelerating can mean deceleration as well
  // we are combining accel and decel to reduce chances of errors
  Cruising = 2
};

enum Msg_Type
{
  None = 0,
  Telemetry = 1,
  Accelerate = 2
  // note: accelerating can mean deceleration as well
  // we are combining accel and decel to reduce chances of errors
};

enum Nav_Msg_Type
{
  Position = 0x00,
  Velocity = 0x01,
  Acceleration = 0x02
};

static struct States
{
  uint8_t movement_state = Status::Still;
  double position[3] = {0, 0, 0};
  double velocity[3] = {0, 0, 0};
  double accel[3] = {0, 0, 0};

  double speed = 0;
  double desired_speed = 0;
  double desired_accel = 0;
  bool ready_to_send = false;
  bool fault_detected = false;
  uint32_t fault_type = FaultType::No_Fault;
  uint8_t msg_type = Msg_Type::None;
  bool routine_just_changed = false; // flag to see if routine was JUST changed
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

bool is_just_changed(struct States *context)
{
  return context->routine_just_changed;
}

void switch_just_changed(struct States *context, bool just_changed)
{
  context->routine_just_changed = just_changed;
}

uint8_t get_msg_type(struct States *context)
{
  return context->msg_type;
}

void set_msg_type(struct States *context, uint8_t msg_type)
{
  context->msg_type = msg_type;
}

// to-do: fit canbus messages here
double get_position()
{
  return 0;
}

double get_speed(struct States *context)
{
  return context->speed;
}

void set_speed(struct States *context, double speed)
{
  context->speed = speed;
}

void get_velocity(struct States *context, double velocity[3])
{
  for (int8_t i = 0; i < 3; i++)
  {
    velocity[i] = context->velocity[i];
  }
}

void set_velocity(struct States *context, double velocity[3])
{
  for (int8_t i = 0; i < 3; i++)
  {
    context->velocity[i] = velocity[i];
  }
}

double get_acceleration(struct States *context)
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

// indentation changed here for some reason
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
    return -1; // error, fault hasn't been identified
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
    out_msg.buf[i] = 0x0F; // just a placeholder. fill according to situation.
  }

  // out_msg.timeout = wait_time;

  // cbus.write(out_msg);
  return out_msg;
}

THD_WORKING_AREA(waThread1, 128);

static THD_FUNCTION(Thread1, arg)
{
  (void)arg;
  chRegSetThreadName("communication_task");
  while (1)
  {
    // add getter for ready to send flag here
    if (pod_context.ready_to_send)
    {
      // read context file to know request type here
      msg_type = Msg_Type::Telemetry;     // placeholder
      recipient_id = CANID_List::General; // placeholder
      CAN_message_t *out_msg = build_msg(recipient_id, msg_type);
      out_buff.Write(out_msg);
      // Cbus.write(out_msg);
    }

    // begin reading messages
    while (Cbus.available())
    {
      CAN_message_t in_msg;
      Cbus.read(in_msg);
      in_buff.Write(in_msg);

      /*
        Nav Module Message Handling Here:
          IF / STATEMENT 1: Position Change
          ELSE IF / STATEMENT 2: Velocity Change
          ELSE IF / STATEMENT 3: Acceleration Change
      */
      if (in_msg.buf[0] == nav_msg_list[0][0] && in_msg.buf[1] == nav_msg_list[0][1])
      {
        // response for position change here
      }
      else if (in_msg.buf[0] == nav_msg_list[1][0] && in_msg.buf[1] == nav_msg_list[1][1])
      {
        // response for velocity change here
        double velocity[3] = {0, 0, 0};
        for (int8_t i = 0; i < 3; i++)
        {
          velocity[i] == in_msg.buf[2 * i + 3];
          if (in_msg.buf[2 * i + 3] == 1)
          {
            velocity[i] = -velocity[i];
          }
        }
        double speed = sqrt(velocity[0]**2+velocity[1]**2+velocity[2]**2);

        // set_velocity(&pod_context, &velocity);
        set_speed(&pod_context, speed);
      }
    }

    // unpacking messages
    for (size_t i = 0; i < in_buff.numElements(); i++)
    {
      CAN_message_t in_msg = in_buff.Read();

      // here, check the contents of the message
      msg_type == Msg_Type::Telemetry // placeholder. change.
          if (msg_type == Msg_Type::Telemetry)
      {
        // handle each CANbus member differently
        // add to context, and pass info to UDP module via context
      }
      elif (msg_type == Msg_Type::Fault)
      {
        // handle each CANbus member differently
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
    // add task code here
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
    // start by checking if any requests have been made
    if (movement_request)
    {
      bool fault_detected = is_fault(&pod_context);
      if (fault_detected)
      {
        // stop motor
      }
      uint8_t msg_type = get_msg_type(&pod_context);
      message_read = true;
      set_msg_type(&pod_context, Msg_Type::None);

      // this switch should only TRIGGER movement routine flags
      // it should not contain code on running the routines!
      switch (msg_type)
      {
        /*
        we will not do anything when we get a telem message
        besides continue the same routine as last time.
        */

      case Msg_Type::Accelerate:
        set_state(&pod_context, MovementState::Accelerating);
        switch_just_changed(&pod_context, true);
        break;

      default:
        break;
      }

      uint8_t movement_state = get_state(&pod_context);
      switch (movement_state)
      {
      case MovementState::Accelerating:
        double desired_speed = get_desired_speed(&pod_context);
        double desired_accel = get_desired_accel(&pod_context);

        // TO-DO: we must also trigger flag for speed request from bamocar
        double speed = get_speed(&pod_context);

        // checks if state needs to be changed to cruise
        // speed must be within 2ms^-1 within desired speed to activate
        // cruise
        if (speed < (desired_speed + 2) && speed > (desired_speed - 2))
        {
          set_state(&pod_context, MovementState::Cruising);
          break;
        }

        // accel/decel routine
        if (desired_speed > speed)
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
        double desired_speed = get_desired_speed(&pod_context);
        double desired_accel = get_desired_accel(&pod_context);

        // TO-DO: we must also trigger flag for speed request from bamocar
        double speed = get_speed(&pod_context);

        // checks if state needs to be changed to cruise
        // speed must be within 2ms^-1 within desired speed to activate
        // cruise
        if (speed < (desired_speed + 2) && speed > (desired_speed - 2))
        {
          set_state(&pod_context, MovementState::Cruising);
          break;
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

    chThdYield();
  }
}

void setup()
{
  Can0.begin();
  Can0.setBaudRate(500000);
  Can0.setMaxMB(16);

  messageCreator.buildRcvMessages(rcv_msg_list);
  messageCreator.buildSndMessages(snd_msg_list);
  messageCreator.buildNavMessages(nav_msg_list);

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