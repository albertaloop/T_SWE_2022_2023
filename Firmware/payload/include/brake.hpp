#ifndef BRAKE_HPP
#define BRAKE_HPP

#include "FlexCAN_T4.h"

#include "utils.hpp"

namespace brake_ns {
enum STATE {
	OFF    ,
	ON     ,
};

struct brake_s {
	/* I have no idea what type of break will actually be used.
	 In this case, I have imagined a break whose stopping force is
	 controlled by an analog pin. Something...pressure valve...something*/
	NODE node = BRAKE;
	STATE state = OFF;
	uint8_t pin;  // analog pin to control stopping force
	int force;    // stopping force value between 0-2^16
};

const int MIN = 0;        // in ADC units
const int MAX = 65536-1;  // in ADC units
const int THRESHOLD = 5;  // in ADC units

enum CANBUS_ID {
	/* Mapped CANBUS messages
		At most 256 messages can be defined (0x00 - 0xFF).

		For simplicity, we assume every ID is associated with a
		4 byte payload for both RX and TX
	*/
	OK			= 0x00,  // Generic OK response
	FAIL		= 0x01,  // Generic FAIL response
	ACTIVATE	= 0x02,
	DEACTIVATE	= 0x03,
	READ_ADC	= 0x04,
};

STATE get_state(struct brake_s &);
void activate(struct brake_s &);
void deactivate(struct brake_s &);
int get_value (struct brake_s &brake_s);
FCTP_FUNC void canbus_payload_to_brake(struct brake_s &brake_s, CAN_message_t &msg, CANBus<_bus, _rxSize, _txSize> &can) {
	/* Translate CANBus messages to brake commands */
	CAN_message_t tx_msg;
	int payload;
	int_of_bytes(payload, msg.buf);  // always 4 bytes
	switch (msg.id)
	{
	case (CAN_BRK_ID | ACTIVATE):
		activate(brake_s);
		data_to_canbus_payload(tx_msg, CAN_BRK_ID | OK, IGNORE_VALUE);
		break;
	case (CAN_BRK_ID | DEACTIVATE):
		deactivate(brake_s);
		data_to_canbus_payload(tx_msg, CAN_BRK_ID | OK, IGNORE_VALUE);
		break;
	case (CAN_BRK_ID | READ_ADC):
		int adc_value = get_value(brake_s);
		data_to_canbus_payload(tx_msg, CAN_BRK_ID | READ_ADC, adc_value);
		break;
	}
	can.buffer_outgoing(tx_msg);
}
}

#endif