#include "data_model.h"
#include "priority_queue.h"
#include <string.h>




int is_valid_cmd(char read[CMD_PACKET_LEN]) {
    if((uint8_t)read[0] == CMD_MSG) {
        if((uint8_t)read[1] == CMD_ESTOP || CMD_PREP || CMD_CRAWL || CMD_LAUNCH || CMD_HEALTHCHK) {
            return 1;
        }
    }
    return 0;
}

int get_cmd_prio(uint8_t read[CMD_PACKET_LEN]) {
    if(read[1] == CMD_ESTOP) {
        return PRIO_HIGH;
    } else if(read[1] == CMD_PREP || read[1] == CMD_CRAWL || read[1] == CMD_LAUNCH) {
        return PRIO_LOW;
    } else if(read[1] == CMD_HEALTHCHK) {
        return PRIO_MED;
    }
    else return 1000;
}

uint16_t get_cmd_int(char read[CMD_PACKET_LEN]) {
    uint16_t ret = 0x0000;
    ret |= (read[0] << 8);
    ret |= read[1];
    return ret;
}

void get_cmd_char(uint16_t cmd, char *ret) {
    memset(ret, 0, CMD_PACKET_LEN);
    ret[0] |= (cmd >> 8) & 0x00FF;
    ret[1] |= cmd & 0x00FF;
}

int cmd_ack(uint8_t cmd) {
    if(cmd == CMD_ESTOP) {
        return ACK_ESTOP;
    } else if(cmd == CMD_CRAWL) {
        return ACK_CRAWL;
    } else if(cmd == CMD_PREP) {
        return ACK_PREP;
    } else if(cmd == CMD_LAUNCH) {
        return ACK_LAUNCH;
    } else if(cmd == CMD_HEALTHCHK) {
        return ACK_HEALTHCHK;
    }
    return -1;
}