#ifndef _MSG_H_
#define _MSG_H_

#include <Arduino.h>
#include <FlexCAN_T4.h>

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
    MessageMaster(FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> &PrimaryBus, FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &BamocarBus);
    MessageMaster(FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> &PrimaryBus);
    void setupPrimaryBus();
    void setupBamocarBus();

    void buildSndMessages();
    void buildRcvMessages();
    void buildNavMessages();

    void sendMsgMain(CAN_message_t &outMsg);
    void sendMsgBamocar(CAN_message_t &outMsg);

    void transmissionEnable();
    void transmissionDisable();
};

#endif