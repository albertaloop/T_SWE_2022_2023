#include <Arduino.h>
#include <ChRt.h>
#include <FlexCAN_T4.h>
#include "MessageBuilders.h"
#include "circular_buffer.h"

#define total_responses 255

bool state_changed = false;
bool led_on = false;
bool gui_launch_commanded = false;
bool gui_prepare_launch = false;
bool gui_stop = false;
bool brake_ready = false;
bool motor_ready = false;
bool fault = false;
bool motor_coast = false;

#define NUM_TX_MAILBOXES 2
#define NUM_RX_MAILBOXES 6

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;




uint8_t status;
int32_t x[3] = {1, 1, 1};
// CircularBuffer<CAN_message_t, 10> output_buffer;

unsigned long wait_time = 40; // 40 ms

uint8_t team_id = 0;

void (*responses[255])( void );

// bool systemsCheck() {
//   return true;
// }

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

// UDP communications thread
static THD_FUNCTION(Thread2, arg) {
  (void)arg;
  chRegSetThreadName("UDP thread");
  while (1) {
    Serial.println("UDP thread");


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
  while (1) {
    Serial.print("State thread");
    // switch (status) {
    //     case Fault: {
    //         if(systemsCheck()) {
    //           status = Status::SafeToApproach;
    //           state_changed = true;
    //         }
    //         break;
    //     }
    //     case SafeToApproach: {
    //         if(state_changed) {
    //             CAN_message_t msg_out;
    //             msg_out.id = (uint32_t)0x0fffffff;
    //             msg_out.len = (uint8_t)1;
    //             output_buffer.Write(msg_out);
    //             // state_changed = false;
    //         }
    //         if(gui_prepare_launch) {
    //           status = Status::ReadyToLaunch;
    //           state_changed = true;
    //         } else if (fault) {
    //           status = Status::Fault;
    //           state_changed = true;
    //         }
    //         break;
    //     }
    //     case ReadyToLaunch: {
    //         if(gui_launch_commanded && brake_ready && motor_ready) {
    //           status = Status::Launching;
    //         } else if (fault) {
    //           status = Status::Fault;
    //         }
    //         break;
    //     }
    //     case Launching: {
    //         if(motor_coast) {
    //           status = Status::Coasting;
    //         } else if (fault) {
    //           status = Status::Fault;
    //         } else if (gui_stop) {
    //           status = Status::Braking;
    //         }
    //         break;
    //     }
    //     case Coasting: {
      
    //       break;
    //     }
    //     default:
    //         break;
    // }
    Serial.print("Status = ");
    Serial.println(status);

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