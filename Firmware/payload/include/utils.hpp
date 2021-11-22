#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdint.h>

#include <CircularBuffer.h>
#include "FlexCAN_T4.h"

#define IGNORE_VALUE 0

void int_to_bytes(uint8_t*, const int);
void int_of_bytes(int &, const uint8_t*);
void print_buffer(const CAN_message_t &);

void data_to_canbus_payload(CAN_message_t &, const uint32_t, const int);

enum CAN_ID {
	CAN_BMS_ID = 0x100,
	CAN_NAV_ID = 0x200,
	CAN_BRK_ID = 0x300,
	CAN_MTR_ID = 0x400,
	CAN_OBC_ID = 0x500,
};

enum NODE {
	BMS,
	NAV,
	BRAKE,
	MOTOR,
	OBC
};

struct bms {
	NODE node = BMS;
	int voltage;
	int current;
	int temp;
};

struct navigation {
	NODE node = NAV;
};

struct motor {
	NODE node = MOTOR;
};


FCTP_CLASS class CANBus
{
	FlexCAN_T4<_bus, _rxSize, _txSize> &can;
	CircularBuffer<CAN_message_t, 10> incoming_buffer;
	CircularBuffer<CAN_message_t, 10> outgoing_buffer;
public:
	CANBus(FlexCAN_T4<_bus, _rxSize, _txSize> can_): can(can_) {}
	// CANBus(FlexCAN_T4 *fc, CAN_filter_t *mask);
	bool incoming_empty();
	void clear_incoming();
	uint8_t n_incoming_buffers();
	bool outgoing_empty();
	void clear_outgoing();
	uint8_t n_outgoing_buffers();
	bool buffer_incoming();
	int read(CAN_message_t &);
	bool buffer_outgoing(CAN_message_t &msg_out);
	int send();
};

// CANBus::CANBus(FlexCAN_T4 *fc, CAN_filter_t *mask) {
//     this->can = fc;
//     if(mask == 0) {
//         this->can->begin();
//     } else {
//         this->can->begin(*mask);
//     }
// }

FCTP_FUNC bool CANBus<_bus, _rxSize, _txSize>::incoming_empty() {
	return this->incoming_buffer.isEmpty();
}

FCTP_FUNC void CANBus<_bus, _rxSize, _txSize>::clear_incoming() {
	this->incoming_buffer.clear();
}

FCTP_FUNC bool CANBus<_bus, _rxSize, _txSize>::outgoing_empty() {
	return this->outgoing_buffer.isEmpty();
}

FCTP_FUNC void CANBus<_bus, _rxSize, _txSize>::clear_outgoing() {
	this->outgoing_buffer.clear();
}

FCTP_FUNC uint8_t CANBus<_bus, _rxSize, _txSize>::n_incoming_buffers() {
	return this->incoming_buffer.size();
}

FCTP_FUNC uint8_t CANBus<_bus, _rxSize, _txSize>::n_outgoing_buffers() {
	return this->outgoing_buffer.size();
}

FCTP_FUNC bool CANBus<_bus, _rxSize, _txSize>::buffer_incoming() {
	// pull message from CANBUS and write it to incoming buffer
	CAN_message_t msg_in;
	Serial.print("Buffer incoming received: ");
	print_buffer(msg_in);
	this->can.read(msg_in);
	return this->incoming_buffer.unshift(msg_in);
}

FCTP_FUNC int CANBus<_bus, _rxSize, _txSize>::read(CAN_message_t &msg) {
	// pull message from incoming buffer
	if (!this->incoming_buffer.isEmpty()) {
		CAN_message_t msg = this->incoming_buffer.pop();
		Serial.print("Incoming buffer read: ");
		print_buffer(msg);
		return 0;
	}
	return -1;
}

FCTP_FUNC bool CANBus<_bus, _rxSize, _txSize>::buffer_outgoing(CAN_message_t &msg_out) {
	// write message to outgoing buffer
	Serial.print("Outgoing buffer received: ");
	print_buffer(msg_out);
	return this->outgoing_buffer.unshift(msg_out);
}

FCTP_FUNC int CANBus<_bus, _rxSize, _txSize>::send() {
	// push message from outgoing buffer to CANBUS
	if (!this->outgoing_buffer.isEmpty()) {
		CAN_message_t msg_out = this->outgoing_buffer.pop();
		Serial.print("Outgoing buffer sent: ");
		print_buffer(msg_out);
		this->can.write(msg_out);
		return 0;
	}
	return -1;
}

#endif