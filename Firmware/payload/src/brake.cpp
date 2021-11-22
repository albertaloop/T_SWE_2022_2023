#include "Arduino.h"
#include "FlexCAN_T4.h"

#include "brake.hpp"
#include "utils.hpp"

namespace brake_ns {
STATE get_state(struct brake_s &brake_s) {
	return brake_s.state;
}

int get_value (struct brake_s &brake_s) {
	return analogRead(brake_s.pin);
}

void set(struct brake_s &brake_s, int force) {
	analogWrite(brake_s.pin, force);
	if (force > THRESHOLD)
		brake_s.state = ON;
	else
		brake_s.state = OFF;
}

void activate(struct brake_s &brake_s) {
	if (brake_s.state == OFF)
		set(brake_s, MAX);
}

void deactivate(struct brake_s &brake_s) {
	if (brake_s.state == ON)
		set(brake_s, MIN);
}
};