#ifndef _MESG_H_
#define _MESG_H_

#include <stdint.h>


#define NODE_MC         0
#define NODE_BMS        1
#define NODE_NAV        2

uint8_t status;
uint32_t node_status;

struct {
    int32_t accel;
    int32_t position;
    int32_t velocity;
    int32_t batt1_v;
    int32_t batt1_c;
    int32_t batt1_temp;
    int32_t batt2_v;
    int32_t batt2_c;
    int32_t batt2_temp;
} telemetry;

struct {

} health_check;

struct {


} motor_control;

struct {
    int32_t batt1_v;
    int32_t batt1_c;
    int32_t batt1_temp;
    int32_t batt2_v;
    int32_t batt2_c;
    int32_t batt2_temp;
} bms;


struct {
    int32_t accel;
    int32_t position;
    int32_t velocity;
} navigation;

#endif