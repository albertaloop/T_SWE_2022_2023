#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FlexCAN_T4.h>

#include "ChRt.h"
#include "MessageMaster.h"
#include "StateMachine.h"
#include "BamocarInterface.h"

unsigned long wait_time = 400; // 400 ms
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> Can0;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;

MessageMaster msgMaster(Can0, Can1);
BamocarInterface bamocar(msgMaster);
StateMachine stateMachine(msgMaster, bamocar);


THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg)
{
  (void)arg;
  chRegSetThreadName("communication_task");
  while (1)
  {
    // begin reading messages
    while (Can0.events()>0) // CHECK THIS OUT, POSSIBLY INCORRECT. 
    {
      stateMachine.read_msg();
      delay(wait_time);
      chThdYield();
    }
    stateMachine.msg_tasks();
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
    stateMachine.movement_tasks();
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
  msgMaster.stateMachineMode();
  chSysInit();
  chBegin(chSetup);
}

void loop()
{
  // Serial.println("Main thread");

}