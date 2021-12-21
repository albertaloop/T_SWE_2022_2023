#include <Arduino.h>
#include "MessageMaster.h"
#include <Wire.h>
#include <SPI.h>
#include <FlexCAN_T4.h>

#include "ChRt.h"

FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;

uint8_t CAN_id = 4;

MessageMaster messageCreator(Can0, Can1);

bool movement_request = false;
bool message_read = false;

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

enum CanMessages
{
  Start = 0,
  Position = 9,
  Velocity = 10,
  Acceleration = 11
};

enum BamocarCanID
{
  Transmit = 0x181,
  Receive = 0x201,
};

enum FaultType
{
  No_Fault = 0,
  Gen_Fault = 1,
  Mtr_Fault = 2,
  Nav_Fault = 3,
  Brk_Fault = 4
};

static struct States
{
  uint8_t movement_state = MovementState::Still;
  double position[3] = {0, 0, 0};
  double velocity[3] = {0, 0, 0};
  double accel[3] = {0, 0, 0};

  double speed = 0;
  double desired_speed = 0;
  double desired_accel = 0;
  bool telemetry_request = false;
  bool fault_detected = false;
  uint32_t fault_type = FaultType::No_Fault;
  uint8_t msg_type = CanMessages::Start;
  bool routine_just_changed = false; // flag to see if routine was JUST changed
} pod_context;

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

double get_acceleration_scalar(struct States *context)
{
  return sqrt(context->accel[0] * context->accel[0] + context->accel[1] * context->accel[1] + context->accel[2] * context->accel[2]);
}

void get_acceleration(struct States *context, double accel[3])
{
  for (int8_t i = 0; i < 3; i++)
  {
    accel[i] = context->accel[i];
  }
}

void set_acceleration(struct States *context, double accel[3])
{
  for (int8_t i = 0; i < 3; i++)
  {
    context->accel[i] = accel[i];
  }
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
bool fault_msg(struct States *context, uint8_t CAN_id, uint8_t fault_type)
{
  if (is_fault(context))
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

void *build_msg(CAN_message_t &out_msg, uint8_t msg_id, uint8_t length, uint8_t *buff)
{
  out_msg.id = msg_id;
  out_msg.len = length;
  for (uint8_t i = 0; i < length; i++)
  {
    out_msg.buf[i] = buff[i];
  }
}

THD_WORKING_AREA(waThread1, 128);

static THD_FUNCTION(Thread1, arg)
{
  (void)arg;
  chRegSetThreadName("communication_task");
  while (1)
  {
    // add getter for ready to send flag here
    if (1)
    {
      // read context struct to know request type here
      CAN_message_t out_msg;
      uint8_t x[3] = {1, 3, 45};
      build_msg(out_msg, 1, 3, &x[0]);
      Can0.write(out_msg);
    }

    // begin reading messages
    Can0.events();
    CAN_message_t in_msg;
    while (Can0.read(in_msg)) // CHECK THIS OUT, LIKELY TO BE INCORRECT
    {
      Can0.events();
      /*
        Nav Module Message Handling Here:
          IF / STATEMENT 1: Position Change
          ELSE IF / STATEMENT 2: Velocity Change
          ELSE IF / STATEMENT 3: Acceleration Change
      */
      if (in_msg.id == CanMessages::Position)
      {
        // response for position change here
      }
      else if (in_msg.id == CanMessages::Velocity)
      {
        // response for velocity change here
        double velocity[3] = {0, 0, 0};
        for (int8_t i = 0; i < 3; i++)
        {
          velocity[i] = in_msg.buf[2*i+1];
          if (in_msg.buf[2*i+1] == 1)
          {
            velocity[i] = -velocity[i];
          }
        }
        double speed = sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);

        set_velocity(&pod_context, velocity);
        set_speed(&pod_context, speed);
      }
      else if (in_msg.id == CanMessages::Acceleration)
      {
        // response for velocity change here
        double accel[3] = {0, 0, 0};
        for (int8_t i = 0; i < 3; i++)
        {
          accel[i] = in_msg.buf[2 * i + 3];
          if (in_msg.buf[2 * i + 3] == 1)
          {
            accel[i] = -accel[i];
          }
        }
        set_acceleration(&pod_context, accel);
      }

      /*
        Message Creation Here.
        Here we check the state of the context object:
          STATEMENT 1: If ready for telemetry 
          STATEMENT 2: Velocity Change
          STATEMENT 3: Acceleration Change
      */

      // // unpacking messages
      // for (size_t i = 0; i < in_buff.numElements(); i++)
      // {
      //   CAN_message_t in_msg = in_buff.Read();

      //   // here, check the contents of the message
      //   msg_type == Msg_Type::Telemetry // placeholder. change.
      //       if (msg_type == Msg_Type::Telemetry)
      //   {
      //     // handle each CANbus member differently
      //     // add to context, and pass info to UDP module via context
      //   }
      //   elif (msg_type == Msg_Type::Fault)
      //   {
      //     // handle each CANbus member differently
      //     recipient_id = CANID_List::General;
      //     CAN_message_t *out_msg = build_msg(0, msg_type);
      //     out_buff.Write(out_msg);
      //   }
      // }
      delay(wait_time);
      chThdYield();
    }
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

    // CAN_message_t in_msg;
    // Serial.print(Can0.read(in_msg));
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
    CAN_message_t out_msg;
    
    // start by checking if any faults
    bool fault_detected = is_fault(&pod_context);
    if (fault_detected)
    {
      // INSERT: stop motor

    }

    // this switch contains code on running the routines!
    uint8_t movement_state = get_state(&pod_context);
    switch (movement_state)
    {
    case MovementState::Accelerating:
      double desired_speed = get_desired_speed(&pod_context);
      double desired_accel = get_desired_accel(&pod_context);

      // TO-DO: we must also add bamocar messages
      double speed = get_speed(&pod_context);

      // checks if state needs to be changed to cruise
      // speed must be within 2ms^-1 within desired speed to activate
      // cruise
      if (speed < (desired_speed + 2) && speed > (desired_speed - 2))
      {
        set_state(&pod_context, MovementState::Cruising);
        //trigger message asking bamocar to cut accel
      }

      // accel/decel routine
      else if (desired_speed > speed)
      {
        // trigger message asking bamocar to accelerate
        build_msg(out_msg, BamocarCanID::Receive, )
      }

      // decel routine
      else if (desired_speed < speed)
      {
        // trigger message asking bamocar to decelerate
      }
      break;

    case MovementState::Cruising:
      desired_speed = get_desired_speed(&pod_context);
      desired_accel = get_desired_accel(&pod_context);

      // TO-DO: we must also trigger flag for speed request from bamocar
      speed = get_speed(&pod_context);

      // checks if state needs to be changed to cruise
      // speed must be within 2ms^-1 within desired speed to activate
      // cruise
      if (speed < (desired_speed + 2) && speed > (desired_speed - 2))
      {
        set_state(&pod_context, MovementState::Cruising);
        
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

    chThdYield();
  }
}

void chSetup() {
  chThdCreateStatic(waThread1, sizeof(waThread1),
                    NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2),
                    NORMALPRIO, Thread2, NULL);
  chThdCreateStatic(waThread3, sizeof(waThread3),
                    NORMALPRIO, Thread2, NULL);
}

void setup()
{
  chSysInit();
  chBegin(chSetup);
}

void loop()
{
  // Serial.println("Main thread");

}