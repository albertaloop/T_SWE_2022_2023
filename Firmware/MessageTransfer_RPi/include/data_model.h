#ifndef _MESG_H_
#define _MESG_H_

#include <stdint.h>

#define NODE_MC         0
#define NODE_BMS        1
#define NODE_NAV        2

#define CMD_PACKET_LEN 2

#define CMD_MSG           0xC0
#define CMD_ESTOP         0xC0
#define CMD_PREP          0xC2
#define CMD_LAUNCH        0xC4
#define CMD_CRAWL         0xC6
#define CMD_HEALTHCHK     0xC8

#define ACK_MSG           0xA0
#define ACK_ESTOP         0xA0
#define ACK_PREP          0xA2
#define ACK_LAUNCH        0xA4
#define ACK_CRAWL         0xA6
#define ACK_HEALTHCHK     0xA8

#define TLM_BCAST_DELAY 2000
#define UART_TIMEOUT 100
#define CMD_PROCESS_TIME 2000 // ms



int is_valid_cmd(char read[CMD_PACKET_LEN]);
int get_cmd_prio(uint8_t read[CMD_PACKET_LEN]);
uint16_t get_cmd_int(char read[CMD_PACKET_LEN]);
void get_cmd_char(uint16_t cmd, char *ret);
int cmd_ack(uint8_t cmd);

#endif