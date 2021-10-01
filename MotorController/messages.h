#include <Arduino.h>

//This is our list of messages

uint8_t[3][7] rcv_msg_list = {};
uint8_t[10][7] snd_msg_list = {};

//arr[0] is the length of the msg
//arr[1-n] are the msg bytes

//change the 0 in snd_msg_list[0][i] = arr[i]; to the msg num
//0xFF is a placeholder for values we will not send

//Transmission messages
int8_t[4] arr = {0x3, 0x3D, 0xE2, 0x00};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[0][i] = arr[i];
}

uint8_t[4] arr = {0x3, 0x51, 0x04, 0x00};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[1][i] = arr[i];
}

uint8_t[4] arr = {0x3, 0x3D, 0xE8, 0x00};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[2][i] = arr[i];
}

uint8_t[4] arr = {0x3, 0x51, 0x00, 0x00};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[3][i] = arr[i];
}

uint8_t[4] arr = {0x3, 0x35, 0xF4, 0x01};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[4][i] = arr[i];
}

uint8_t[4] arr = {0x3, 0xED, 0xE8, 0x03};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[5][i] = arr[i];
}

uint8_t[4] arr = {0x3, 0x31, 0xD4, 0x03};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[6][i] = arr[i];
}

uint8_t[4] arr = {0x3, 0x3D, 0x30, 0x64};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[7][i] = arr[i];
}

uint8_t[4] arr = {0x3, 0x31, 0xA4, 0x7F};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[8][i] = arr[i];
}

uint8_t[4] arr = {0x3, 0x31, 0x00, 0x00};
for (uint8_t i = 0; i < arr[0]; i++)
{
    snd_msg_list[9][i] = arr[i];
}

// Recieve messages
uint8_t[5] arr = {0x4, 0xE2, 0x01, 0x00, 0x00};
for (uint8_t i = 0; i < arr[0]; i++)
{
    rcv_msg_list[0][i] = arr[i];
}

uint8_t[5] arr = {0x4, 0xE8, 0x01, 0x00, 0x00};
for (uint8_t i = 0; i < arr[0]; i++)
{
    rcv_msg_list[1][i] = arr[i];
}

uint8_t[5] arr = {0x4, 0x30, 0xFF, 0xFF, 0xFF};
for (uint8_t i = 0; i < arr[0]; i++)
{
    rcv_msg_list[2][i] = arr[i];
}