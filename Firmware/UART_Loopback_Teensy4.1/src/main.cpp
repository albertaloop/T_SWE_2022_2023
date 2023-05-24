#include <Arduino.h>
#include <ChRt.h>
#include <FlexCAN_T4.h>
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


#define USART1 Serial1



#define NUM_TX_MAILBOXES 2
#define NUM_RX_MAILBOXES 6

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

int recv_buf[2] = {0, 0};


THD_WORKING_AREA(waThread1, 128);

// CANbus communications
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  chRegSetThreadName("CANbus thread");
  while (1) {
    Serial.println("CAN thread");
    delay(1000);
    chThdYield();
  }
}



THD_WORKING_AREA(waThread3, 128);

// State machine
static THD_FUNCTION(Thread3, arg) {
  (void)arg;
  chRegSetThreadName("State thread");
  while (1) {
    Serial.println("State thread");
    delay(1000);
    chThdYield();
  }
}

void chSetup() {
  chThdCreateStatic(waThread1, sizeof(waThread1),
    NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread3, sizeof(waThread3),
    NORMALPRIO, Thread3, NULL);
}

void setup() {
  // chSysInit();
  // chBegin(chSetup);
  Serial.begin(9600);
  USART1.begin(9600);
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
}

void loop() {
  Serial.println("Main thread");
  USART1.write(ACK_MSG);
  USART1.write(CMD_ESTOP);
  delay(1000);
  // while(USART1.available() == 0);
  recv_buf[0] = USART1.read();
  // while(USART1.available() == 0);
  recv_buf[1] = USART1.read();   
  Serial.println(recv_buf[0], HEX);
  Serial.println(recv_buf[1], HEX);

  // delay(wait_time);
  delay(1000);
  // chThdYield();
}     