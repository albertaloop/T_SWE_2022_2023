#include <Arduino.h>

//This is our list of messages

uint8_t[3][7] rcv_msg_list = {};
uint8_t[11][7] snd_msg_list = {};

//arr[0] is the length of the msg
//arr[1-n] are the msg bytes
uint8_t[4] arr = {0x3, 0x3D, 0xE2, 0x00};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[0][i] = arr[i];
}

uint8_t[4] arr = {0x3, 0x3D, 0xE2, 0x00};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[0][i] = arr[i];
}