#include "CANBus.h"
#include "circular_buffer.h"

#define PACKET_LENGTH 8
#define ACCELERATION_IDX 2
#define POSITION_IDX 6
#define VELOCITY_IDX 10
#define BATTERY_VOLTAGE_IDX 14
#define N_MODULES 4

//config script made with help of
//http://eet.etec.wwu.edu/KurtTaylor/project/docs/Bibleography/MCU/ConnSoftDocs/Kinetis%20SDK%20v2.0.0%20API%20Reference%20Manual/group__flexcan__driver.html
flexcan_config_t flexcanConfig;

FLEXCAN_GetDefaultConfig(&flexcanConfig);
FLEXCAN_Init(CAN_SETUP, &flexcanConfig);
FLEXCAN_Enable(CAN_SETUP, true);

// FlexCAN Cbus(500000);
// CircularBuffer<char, 10> buff;

uint16_t wait_time = 40; // 40 ms

uint8_t CAN_id = 0;

bool fault_detected = false;
bool ready_to_send = false;

// static struct _CBUS_context
// {
//   tiny_state_ctx state; /**< Must be the first element */
//   uint32_t pod_state;
//   uint32_t fault_detected;
// } CBUS_context;

enum Status {
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
  Gen_Fault = 0,
  Mtr_Fault = 1,
  Nav_Fault = 2,
  Brk_Fault = 3
};

uint8_t status = Status::SafeToApproach;

double position = 0;
double velocity = 0;
double acceleration = 0;

// TODO: implement this function
uint8_t get_status() {
  return 0;
}

// TODO: implement this function
uint32_t get_position() {
  return 2334566;
}

// TODO: implement this function
uint32_t get_velocity() {
  return 13454634;
}

// TODO: implement this function
uint32_t get_acceleration() {
  return 435468763;
}

uint8_t *fault_msg(uint8_t CAN_id, uint8_t fault_type)
{
  if (fault_detected) {
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

// example code for adding context
// static void wifi_state_update(void *ctx, uint32_t next, uint32_t wait)
// {
//   struct _g_wifi_context *pCtx = ctx;

//   WIFI_PRINTF("%s(%u) -> %s(%u)\r\n", tiny_state_name(ctx, pCtx->state.state),
//               pCtx->state.state, tiny_state_name(ctx, next), next);

//   /* Set the new state */
//   tiny_state_update(ctx, next);

//   /* Set the holdoff/wait */
//   wifi_counter_set(wait);
// }

void setup() {
  FlexCAN Cbus(500000);
  Cbus.begin();
  CircularBuffer<CAN_message_t, 10> in_buff, out_buff;
}

void loop() {

  //add getter for ready to send flag here

  if (ready_to_send)
  {
    //read context file to know request type here
    msg_type = Msg_Type::Telemetry; //placeholder
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
}