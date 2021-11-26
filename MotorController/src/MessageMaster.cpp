#include "MessageMaster.h"
#include <Arduino.h>

// NOTE: For the Bamocar, percentages are provided as a number ranging from 0
// to 32767.
// For example, 50% is 16383

/* This is our list of receiving messages
receive messages are 4 bytes long!
Returns: array of messages */
void MessageMaster::buildRcvMessages(uint8_t rcv_msg_list[3][5])
{
    //temp messages array
    uint8_t messages[3][5] = {{0x4, 0xE2, 0x01, 0x00, 0x00},
                                {0x4, 0xE8, 0x01, 0x00, 0x00},
                                {0x4, 0x30, 0xFF, 0xFF, 0xFF}};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            rcv_msg_list[i][j] = messages[i][j];
        }
    }
}

/* This is our list of sending messages
receive messages are 3 bytes long!
Returns: array of messages */
void MessageMaster::buildSndMessages(uint8_t snd_msg_list[9][4])
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
            snd_msg_list[i][j] = messages[i][j];
        }
    }
}


//this is the list of messages which can be received from the nav module
void MessageMaster::buildNavMessages(uint8_t nav_msg_list[3][2])
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
            nav_msg_list[i][j] = messages[i][j];
        }
    }
}
