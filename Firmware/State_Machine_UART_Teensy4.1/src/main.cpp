#include <Arduino.h>
#include <ChRt.h>
#include <FlexCAN_T4.h>
#include "MessageBuilders.h"
#include "circular_buffer.h"

#define CMD_MSG           0xC0
#define CMD_ESTOP         0xC0
#define CMD_PREP          0xC2
#define CMD_LAUNCH        0xC4
#define CMD_CRAWL         0xC6
#define CMD_HEALTHCHK     0xC8

#define ACK_MSG           0xA0
#define ACK_ESTOP         0xA0
#define ACK_PREP          0xA2
#define ACK_LAUNCH        0xA4
#define ACK_CRAWL         0xA6
#define ACK_HEALTHCHK     0xA8

#define total_responses 255

#define USART1 Serial1

bool state_changed = false;
bool led_on = false;
bool gui_launch_commanded = false;
bool gui_prepare_launch = false;
bool gui_stop = false;
bool brake_ready = false;
bool motor_ready = false;
bool fault = false;
bool motor_coast = false;
uint32_t node_status;

#define NUM_TX_MAILBOXES 2
#define NUM_RX_MAILBOXES 6

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

CircularBuffer<int, 10> usart_recv_cbuf;



uint8_t status;
int32_t x[3] = {1, 1, 1};
// CircularBuffer<CAN_message_t, 10> output_buffer;

unsigned long wait_time = 40; // 40 ms

uint8_t team_id = 0;

void (*responses[255])( void );

bool systemsCheck() {
  // Sound off CAN nodes
  // Check for valid ranges of sensor data
  return true;
}

int get_cmd_ack(int cmd) {
  if(cmd == CMD_ESTOP) {
    return ACK_ESTOP;
  } else if (cmd == CMD_PREP) {
    return ACK_PREP;
  } else if (cmd == CMD_LAUNCH) {
    return CMD_LAUNCH;
  } else if (cmd == CMD_CRAWL) {
    return ACK_CRAWL;
  } else if (cmd == CMD_HEALTHCHK) {
    return ACK_HEALTHCHK;
  } else {
    Serial.println("Invalid CMD");
    return -1;
  }
}

// USART functions
void receive_cmds(int * acked_cmd) {
  int recv_buf[2];
  for(int i = 0; i < 10; i++) {
    if(USART1.available() > 0) {
      recv_buf[0] = USART1.read();
      recv_buf[1] = USART1.read();
      if(recv_buf[0] != -1 && recv_buf[1] != -1) {
        if(recv_buf[0] == CMD_MSG) {
          // Command received
          if(*acked_cmd != recv_buf[1]) {
            *acked_cmd = recv_buf[1];
            usart_recv_cbuf.Write(recv_buf[1]);
            int cmd_ack = get_cmd_ack(recv_buf[1]);
            USART1.print(ACK_MSG);
            USART1.print(cmd_ack);
          }
        }
      }
    }
    else {
      break;
    }
  }
}
void send_telemetry() {

}


// State functions
void state_fn(void(*fn)(int i)) {
  int cmd_msg;
  while(!usart_recv_cbuf.isEmpty()) {
    cmd_msg = usart_recv_cbuf.Read();
    fn(cmd_msg);
  }
}
void fault_state(int cmd_msg) {
  if(systemsCheck()) {
    status = Status::SafeToApproach;
    state_changed = true;
  }
}
void safe_to_approach(int cmd_msg) {
  if(state_changed) {
      // CAN_message_t msg_out;
      // msg_out.id = (uint32_t)0x0fffffff;
      // msg_out.len = (uint8_t)1;
      // output_buffer.Write(msg_out);
      // state_changed = false;
  }
  if(cmd_msg == CMD_PREP) {
    status = Status::ReadyToLaunch;
    state_changed = true;
  } else if (cmd_msg == CMD_ESTOP) {
    status = Status::Fault;
    state_changed = true;
  }
}
void ready_to_launch(int cmd_msg) {
  if(gui_launch_commanded && brake_ready && motor_ready) {
    status = Status::Launching;
  } else if (fault) {
    status = Status::Fault;
  }
}
void launching(int cmd_msg) {
  if(motor_coast) {
    status = Status::Coasting;
  } else if (fault) {
    status = Status::Fault;
  } else if (gui_stop) {
    status = Status::Braking;
  }
}
void coasting(int cmd_msg) {

}

THD_WORKING_AREA(waThread1, 128);

// CANbus communications
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  chRegSetThreadName("CANbus thread");
  while (1) {
      CAN_message_t rcv_msg;
    // for(int i = 0; i < 10; i++) {
      // if(can1.read(rcv_msg)) {
      //   // responses[(rcv_msg.id >> 30)]();
      //   Serial.println("msg read");
      // }
      Serial.println("can threads");
        // CAN_message_t send_msg;

    // for(int i = 0; i < 10; i++) {
    //     if(output_buffer.isEmpty()){
    //       break;
    //     }
    //     // send_msg = output_buffer.Read();
    //     Serial.println(send_msg.id);
    //     // can1.write(MB6, send_msg);    
    //     delay(200);

    // }
    delay(200);

    chThdYield();
  }
}

THD_WORKING_AREA(waThread2, 128);

// UART communications thread
static THD_FUNCTION(Thread2, arg) {
  (void)arg;
  chRegSetThreadName("UART thread");
  int acked_cmd;
  while (1) {
    Serial.println("UART thread");
    receive_cmds(&acked_cmd);
    send_telemetry();

    delay(wait_time);
    delay(100);
    chThdYield();
  }
}

THD_WORKING_AREA(waThread3, 128);

// State machine
static THD_FUNCTION(Thread3, arg) {
  (void)arg;
  chRegSetThreadName("State thread");
  Serial.print("Status = ");
  Serial.println(status);
  while (1) {
    Serial.print("State thread");
    switch (status) {
        case Fault: {
            state_fn(fault_state);
            break;
        }
        case SafeToApproach: {
            state_fn(safe_to_approach);
            break;
        }
        case ReadyToLaunch: {
            state_fn(ready_to_launch);
            break;
        }
        case Launching: {
            state_fn(launching);
            break;
        }
        case Coasting: {
            state_fn(coasting);
            break;
        }
        default:
            break;
    }
    


    delay(1);
    chThdYield();
  }
}

void chSetup() {
  chThdCreateStatic(waThread1, sizeof(waThread1),
    NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2),
    NORMALPRIO, Thread2, NULL);
  chThdCreateStatic(waThread3, sizeof(waThread3),
    NORMALPRIO, Thread3, NULL);
}

void setup() {
  chSysInit();
  chBegin(chSetup);
  status = Status::Fault;
  // init_CAN_messages();

  Serial.begin(9600);
  USART1.begin(115200);
  delay(400);
  can1.begin();
  can1.setBaudRate(1000000);
  can1.setMaxMB(NUM_TX_MAILBOXES + NUM_RX_MAILBOXES);
    for (int i = 0; i<NUM_RX_MAILBOXES; i++){
    can1.setMB((FLEXCAN_MAILBOX)i,RX,EXT);
  }
  for (int i = NUM_RX_MAILBOXES; i<(NUM_TX_MAILBOXES + NUM_RX_MAILBOXES); i++){
    can1.setMB((FLEXCAN_MAILBOX)i,TX,EXT);
  }
  can1.setMBFilter(REJECT_ALL);
  // can1.setMB(MB7,RX,NONE);
  // can1.setMB(MB6,TX,NONE);
  // can1.enableMBInterrupts();
  // can1.setMBFilterRange(MB7, (uint32_t)0x03ffffff, (uint32_t)0x04ffffff); // Mailbox will attept to receive only frames 0x1 to 0x5.
  // can1.mailboxStatus();
}

void loop() {
  Serial.println("Main thread");
  // Serial.print("nav_recenter.id = ");
  // init_CAN_messages();

  // Serial.println(CAN_messages.nav_recenter.id);
    // can1.events();
    // can1.write(CAN_messages.nav_recenter);
  chThdYield();
}     