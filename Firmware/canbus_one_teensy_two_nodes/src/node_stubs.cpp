#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include "node_stubs.hpp"

bool seeded = false;

#define get_bounded_random_number(min, max) ((rand()%(int)(((max) + 1)-(min)))+ (min))

void update_bms(struct bms &_bms) {
	_bms.voltage = get_bounded_random_number(BMS_MIN_VOLTAGE, BMS_MAX_VOLTAGE);
	_bms.current = get_bounded_random_number(BMS_MIN_CURRENT, BMS_MAX_CURRENT);
	_bms.temp = get_bounded_random_number(BMS_MIN_TEMP, BMS_MAX_TEMP);
}

int stringify_bms(uint8_t* buffer, struct bms &_bms) {
	return sprintf((char *) buffer, "BMS Voltage: %d. Current: %d. Temperature: %d", _bms.voltage, _bms.current, _bms.temp);
}

void seed_random(int seed) {
	srand((unsigned) seed);
	seeded = true;
}