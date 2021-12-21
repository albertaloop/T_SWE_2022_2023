#include "MessageMaster.h"
#include <FlexCAN_T4.h>
#include <Arduino.h>

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

// this constructor is used generally for when only one CANbus line is used
MessageMaster::MessageMaster(FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> &PrimaryBus)
{
    this->PrimaryBus = &PrimaryBus;
    boolean dualBus = false;

    this->setupPrimaryBus();
}

//this constructor is only used for when both Bamocar and main line communications are required
MessageMaster::MessageMaster(FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> &PrimaryBus, FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> &BamocarBus)
{
    this->PrimaryBus = &PrimaryBus;
    this->BamocarBus = &BamocarBus;
    boolean dualBus = true;

    this->setupPrimaryBus();
    this->setupBamocarBus();
}

void MessageMaster::setupPrimaryBus() {
    this->buildNavMessages();
    this->buildRcvMessages();
    this->buildSndMessages();

    this->PrimaryBus->begin();
    this->PrimaryBus->setBaudRate(500000);
    this->PrimaryBus->distribute();
    this->PrimaryBus->enableMBInterrupts();
    this->PrimaryBus->mailboxStatus();
}

void MessageMaster::setupBamocarBus()
{
    this->BamocarBus->begin();
    this->BamocarBus->setBaudRate(500000);
    this->BamocarBus->distribute();
    this->BamocarBus->enableMBInterrupts();
    this->BamocarBus->mailboxStatus();
}

// NOTE: For the Bamocar, percentages are provided as a number ranging from 0
// to 32767.
// For example, 50% is 16383

/* This is our list of receiving messages
receive messages are 4 bytes long!
Returns: array of messages */
void MessageMaster::buildRcvMessages()
{
    //temp messages array
    uint8_t messages[3][5] = {{0x4, 0xE2, 0x01, 0x00, 0x00},
                                {0x4, 0xE8, 0x01, 0x00, 0x00},
                                {0x4, 0x30, 0xFF, 0xFF, 0xFF}};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            this->rcv_msg_list[i][j] = messages[i][j];
        }
    }
}

/* This is our list of sending messages
receive messages are 3 bytes long!
Returns: array of messages */
void MessageMaster::buildSndMessages()
{
    // arr[0] is the length of the msg
    // arr[1-n] are the msg bytes

    // change the 0 in snd_msg_list[0][i] = arr[i]; to the msg num
    // 0xFF is a placeholder for values we will not send

    // temp messages array
    uint8_t messages[9][4] = {{0x3, 0x3D, 0xE2, 0x00},
                              {0x3, 0x51, 0x04, 0x00},
                              {0x3, 0x3D, 0xE8, 0x00},
                              {0x3, 0x51, 0x00, 0x00},
                              {0x3, 0x35, 0xF4, 0x01},
                              {0x3, 0xED, 0xE8, 0x03},
                              {0x3, 0x31, 0xD4, 0x03},
                              {0x3, 0x3D, 0x30, 0x64},
                              {0x3, 0x31, 0x00, 0x00}};
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->snd_msg_list[i][j] = messages[i][j];
        }
    }
}


//this is the list of messages which can be received from the nav module
void MessageMaster::buildNavMessages()
{
    // temp messages array
    uint8_t messages[3][2] = {{0x02, 0x00}, //position
                              {0x02, 0x01}, //velocity
                              {0x02, 0x02}  //accel
                              };
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            this->nav_msg_list[i][j] = messages[i][j];
        }
    }
}

void MessageMaster::sendMsgMain(CAN_message_t &outMsg)
{
    (this->PrimaryBus)->write(outMsg);
}

void MessageMaster::sendMsgBamocar(CAN_message_t &outMsg)
{
    (this->BamocarBus)->write(outMsg);
}

void MessageMaster::transmissionEnable()
{
    CAN_message_t outMsg;
    outMsg.id = BamocarCanID::Receive;
    outMsg.len = this->snd_msg_list[0][0];
    for (int i = 0; i < this->snd_msg_list[1][0]; i++)
    {
        outMsg.buf[i] = this->snd_msg_list[0][i+1];
    }
    this->sendMsgBamocar(outMsg);
}

void MessageMaster::transmissionDisable()
{
    CAN_message_t outMsg;
    outMsg.id = BamocarCanID::Receive;
    outMsg.len = this->snd_msg_list[1][0];
    for (int i = 0; i < this->snd_msg_list[1][0]; i++)
    {
        outMsg.buf[i] = this->snd_msg_list[1][i+1];
    }
    this->sendMsgBamocar(outMsg);
}