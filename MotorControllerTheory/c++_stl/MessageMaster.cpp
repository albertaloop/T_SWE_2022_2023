#include <MessageMaster.h>
using namespace std;

/* This is our list of receiving messages
receive messages are 4 bytes long!
Returns: vector of messages */
MessageMaster::vector<vector<uint8_t>> buildRcvMessages() {
    vector<vector<uint8_t>> rcv_msg_list;
    rcv_msg_list.reserve(3);

    rcv_msg_list[0] = vector<uint8_t>{0xE2, 0x01, 0x00, 0x00};
    rcv_msg_list[1] = vector<uint8_t>{0xE8, 0x01, 0x00, 0x00};
    rcv_msg_list[2] = vector<uint8_t>{0x30, 0xFF, 0xFF, 0xFF};

    return rcv_msg_list;
}

/* This is our list of sending messages
receive messages are 3 bytes long!
Returns: vector of messages */
MessageMaster::vector<vector<uint8_t>> buildSndMessages()
{
    vector<vector<uint8_t>> snd_msg_list;
    snd_msg_list.reserve(3);

    snd_msg_list[0] = vector<uint8_t>{0x3D, 0xE2, 0x00};
    snd_msg_list[1] = vector<uint8_t>{0x51, 0x04, 0x00};
    snd_msg_list[2] = vector<uint8_t>{0x3D, 0xE8, 0x00};
    snd_msg_list[3] = vector<uint8_t>{0x51, 0x00, 0x00};
    snd_msg_list[4] = vector<uint8_t>{0x35, 0xF4, 0x01};
    snd_msg_list[5] = vector<uint8_t>{0xED, 0xE8, 0x03};
    snd_msg_list[6] = vector<uint8_t>{0x31, 0xD4, 0x03};
    snd_msg_list[7] = vector<uint8_t>{0x3D, 0x30, 0x64};
    snd_msg_list[8] = vector<uint8_t>{0x31, 0x00, 0x00};

    return snd_msg_list;
}