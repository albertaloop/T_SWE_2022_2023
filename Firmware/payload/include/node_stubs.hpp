#ifndef NODE_STUBS_HPP
#define NODE_STUBS_HPP

#include "brake.hpp"
#include "utils.hpp"

const int BMS_MIN_VOLTAGE = 10000;       // mVolts
const int BMS_MAX_VOLTAGE = 12000;       // mVolts
const int BMS_MIN_CURRENT = 100;         // mAmps
const int BMS_MAX_CURRENT = 3000;        // mAmps
const int BMS_MIN_TEMP = 15;             // Celcius
const int BMS_MAX_TEMP = 40;             // Celcius
const int BRAKE_MIN = 0;                 // in ADC units
const int BRAKE_MAX = 65536-1;           // in ADC units

void update_bms(struct bms &);
// void update_brake(struct brake_s &);
int stringify_bms(uint8_t* buffer, struct bms &_bms);
void seed_random(int seed);

#endif