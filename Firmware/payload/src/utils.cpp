#include <cstring>
#include <stdint.h>

#include "FlexCAN_T4.h"

#include "utils.hpp"

void int_to_bytes(uint8_t* a, const int i) {
	/* Convert 32-bit integer "i" to a four byte, little endian array "a"

		Example: 1234567890, which is 01001001 10010110 00000010 11010010
		returns [11010010, 00000010, 10010110, 01001001]

		Another way to potentially do this (does not work as is)
		a = static_cast<uint8_t*>(static_cast<void*>(&i));
	*/
	a[0] = (i & 0x000000ff);
	a[1] = (i & 0x0000ff00) >> 8;
	a[2] = (i & 0x00ff0000) >> 16;
	a[3] = (i & 0xff000000) >> 24;
}

void int_of_bytes(int &i, const uint8_t* a) {
	/* Convert little-endian byte array to integer */
	memcpy(&i, a, sizeof(i));
}

void print_buffer(const CAN_message_t &msg) {
	/* Example printout (6 bytes):
		ID: 100 Buffer: A 2 0 0 31 2D
	*/
	Serial.print(" ID: "); Serial.print(msg.id, HEX);
	Serial.print(" Buffer: ");
	for ( uint8_t i = 0; i < msg.len; i++ ) {
	  Serial.print(msg.buf[i], HEX); Serial.print(" ");
	} Serial.println();
}

void data_to_canbus_payload(CAN_message_t &msg, const uint32_t id, const int value) {
	/* Encodes a four-byte payload to msg for sending over CANBUS */
	msg.id = id;
	msg.len = 4;
	int_to_bytes(msg.buf, value);
}
