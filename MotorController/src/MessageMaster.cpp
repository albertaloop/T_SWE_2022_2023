#include "MessageMaster.h"
#include <FlexCAN_T4.h>
#include <Arduino.h>

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
    this->stateMachineMode();
}

void MessageMaster::setupPrimaryBus() {
    this->buildNavMessages();
    this->buildRcvMessages();
    this->buildSndMessages();

    this->PrimaryBus->begin();
    this->PrimaryBus->setBaudRate(500000);
    this->PrimaryBus->enableFIFO();
    this->PrimaryBus->enableFIFOInterrupt();

    this->PrimaryBus->setFIFOFilter(REJECT_ALL);
}

void MessageMaster::setupBamocarBus()
{
    this->BamocarBus->begin();
    this->BamocarBus->setBaudRate(500000);
    this->BamocarBus->enableFIFO();
    this->BamocarBus->enableFIFOInterrupt();
    this->BamocarBus->setFIFOFilter(REJECT_ALL);
}

void MessageMaster::stateMachineMode()
{
    this->modeSelected = true;
    this->PrimaryBus->setFIFOFilterRange(0, 9, 11, STD);
    this->PrimaryBus->setFIFOFilterRange(1, 3, 4, STD);
    this->BamocarBus->setFIFOFilter(0, BamocarCanID::Transmit, STD);
}

void MessageMaster::brakeControllerMode()
{
    this->modeSelected = true;
    this->PrimaryBus->setFIFOFilterRange(0, 0, 1, STD);
    this->PrimaryBus->setFIFOFilterRange(0, 6, 7, STD);
}

void MessageMaster::navModuleMode()
{
    this->modeSelected = true;
    this->PrimaryBus->setFIFOFilterRange(0, 0, 1, STD);
    this->PrimaryBus->setFIFOFilter(1, 3, 5, STD);
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
    uint8_t messages[3][2] = {{0x06, 0x00}, //position
                              {0x06, 0x01}, //velocity
                              {0x06, 0x02}  //accel
                              };
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            this->nav_msg_list[i][j] = messages[i][j];
        }
    }
}

/* Send message on main canbus line */
void MessageMaster::sendMsgMain(const CAN_message_t &outMsg)
{
    (this->PrimaryBus)->write(outMsg);
}

/* Send message on canbus line */
void MessageMaster::sendMsgBamocar(const CAN_message_t &outMsg)
{
    (this->BamocarBus)->write(outMsg);
}

/* Sends a message to the Bamocar from the message templates */
void MessageMaster::sendSndTemplateBamocar(int sndArrayRow)
{
    CAN_message_t outMsg;
    outMsg.id = BamocarCanID::Receive; // this is INTENTIONALLY the receive ID. DON'T CHANGE.
    outMsg.len = this->snd_msg_list[sndArrayRow][0];
    for (int i = 0; i < outMsg.len; i++)
    {
        outMsg.buf[i] = this->snd_msg_list[sndArrayRow][i + 1];
    }
    this->sendMsgBamocar(outMsg);
}

void MessageMaster::sendTemplateMain(CanMessages messageID)
{
    CAN_message_t outMsg;
    outMsg.id = messageID;
    outMsg.len = 1;
    outMsg.buf[0] = 1;
    this->sendMsgBamocar(outMsg);
}

/* used by BamocarInterface */
void MessageMaster::transmissionEnableBTB()
{
    this->sendSndTemplateBamocar(0);
}

/* used by BamocarInterface */
void MessageMaster::transmissionEnableHW()
{
    this->sendSndTemplateBamocar(3);
}

/* used by BamocarInterface */
void MessageMaster::transmitNoDisable()
{
    this->sendSndTemplateBamocar(4);
}

/* used by BamocarInterface */
void MessageMaster::transmissionDisable()
{
    this->sendSndTemplateBamocar(1);
}