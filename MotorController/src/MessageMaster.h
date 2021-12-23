#ifndef _MSG_H_
#define _MSG_H_

#include <Arduino.h>
#include <FlexCAN_T4.h>

enum CanMessages
{
  Start = 0,
  Position = 9,
  Velocity = 10,
  Acceleration = 11
};

enum BamocarCanID
{
  Transmit = 0x181, // the address the bamocar will transmit messages from
  Receive = 0x201,  // the address the bamocar will receive messages from
};

/*
  Contains CANbus messages.
*/
class MessageMaster {
  private:
    FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> *PrimaryBus;
    FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> *BamocarBus;
    boolean dualBus;

    uint8_t snd_msg_list[9][4];
    uint8_t rcv_msg_list[3][5];
    uint8_t nav_msg_list[3][2];

  public:
    const uint8_t bamocarTransmitLen = 3;
    const uint8_t bamocarReceiveLen = 4;

    MessageMaster(FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> &PrimaryBus, FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &BamocarBus);
    MessageMaster(FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> &PrimaryBus);
    void setupPrimaryBus();
    void setupBamocarBus();

    void buildSndMessages();
    void buildRcvMessages();
    void buildNavMessages();

    void sendMsgMain(const CAN_message_t &outMsg);
    void sendMsgBamocar(const CAN_message_t &outMsg);
    void sendSndTemplateBamocar(int sndArrayRow);

    void transmissionEnableBTB();
    void transmissionEnableHW();
    void transmitNoDisable();
    void transmissionDisable();
};

#endif