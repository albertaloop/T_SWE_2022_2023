#include <Ethernet2.h>

#define PACKET_LENGTH 34
#define ACCELERATION_IDX 2
#define POSITION_IDX 6
#define VELOCITY_IDX 10
#define BATTERY_VOLTAGE_IDX 14

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress local_ip(192, 168, 1, 177);
IPAddress remote_ip(192, 168, 1, 84);

unsigned int local_port = 3000;
unsigned int remote_port = 3000;

EthernetUDP Udp;

unsigned long wait_time = 40; // 40 ms

uint8_t team_id = 0;

enum Status {
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

uint8_t *build_packet(uint8_t team_id, uint8_t status, int32_t acceleration, int32_t position, int32_t velocity) {
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

void setup() {
  Ethernet.begin(mac, local_ip);
  Udp.begin(local_port);
  Serial.begin(9600);
}

void loop() {
  status = get_status();
  position = get_position();
  velocity = get_velocity();
  acceleration = get_acceleration();

  Udp.beginPacket(remote_ip, remote_port);
  uint8_t *packet = build_packet(team_id, status, (int32_t) acceleration, (int32_t) position, (int32_t) velocity);
  Udp.write(packet, (size_t) PACKET_LENGTH);
  Udp.endPacket();
  delay(wait_time);
}