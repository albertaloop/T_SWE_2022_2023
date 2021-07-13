#include "CANBus.h"
#include "circular_buffer.h"

#define PACKET_LENGTH 34
#define ACCELERATION_IDX 2
#define POSITION_IDX 6
#define VELOCITY_IDX 10
#define BATTERY_VOLTAGE_IDX 14

unsigned long wait_time = 40; // 40 ms

uint8_t team_id = 0;

bool fault_detected = false;
bool ready_to_send = false;

enum Status {
              Fault = 0,
              SafeToApproach = 1,
              ReadyToLaunch = 2,
              Launching = 3,
              Coasting = 4,
              Braking = 5,
              Crawling = 6
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

uint8_t *fault_msg(uint8_t team_id, uint8_t status, int32_t acceleration, int32_t position, int32_t velocity)
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

uint8_t *build_msg(uint8_t team_id, uint8_t status, int32_t acceleration, int32_t position, int32_t velocity) {
  static uint8_t packet[PACKET_LENGTH];
  packet[0] = team_id;
  packet[1] = status;

  for (int i = ACCELERATION_IDX + 3; i >= ACCELERATION_IDX; i--) {
    packet[i] = acceleration & 0xFF;
    acceleration = acceleration >> 8;
  }

  for (int i = POSITION_IDX + 3; i >= POSITION_IDX; i--) {
    packet[i] = position & 0xFF;
    position = position >> 8;
  }

  for (int i = VELOCITY_IDX + 3; i >= VELOCITY_IDX; i--) {
    packet[i] = velocity & 0xFF;
    velocity = velocity >> 8;
  }

  for (int i = BATTERY_VOLTAGE_IDX; i < PACKET_LENGTH; i++) {
    packet[i] = 0;
  }

  return packet;
}


// example code
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
  CANbus Cbus;
}

void loop() {

  if (/* condition */)
  {
    /* code */
  }
  
  Udp.beginPacket(remote_ip, remote_port);
  uint8_t *packet = build_packet(team_id, status, (int32_t) acceleration, (int32_t) position, (int32_t) velocity);
  Udp.write(packet, (size_t) PACKET_LENGTH);
  Udp.endPacket();
  delay(wait_time);
}