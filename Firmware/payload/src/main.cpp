#include <FlexCAN_T4.h>
#include <StensTimer.h>

#include "brake.hpp"
#include "node_stubs.hpp"  // For simulating
#include "utils.hpp"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;  // Only used to simulate OBC
brake_ns::brake_s brake_s_;
CANBus<CAN1, RX_SIZE_256, TX_SIZE_16> can(can1);
CANBus<CAN2, RX_SIZE_256, TX_SIZE_16> obc_can(can2);
#define FAKE_OBC_MESSAGE 1  // Action
#define FAKE_OBC_MESSAGE_MS 1000  // Time between actions in milliseconds
StensTimer* timer;

void timer_callback(Timer* timer){
	Serial.print("Timer call -> Action: ");
	Serial.print(timer->getAction());
	Serial.print(", Current Time: ");
	Serial.println(millis());
	if (timer->getAction() == FAKE_OBC_MESSAGE) {
		CAN_message_t fake_msg;
		fake_msg.id = CAN_BRK_ID | brake_ns::ACTIVATE;
		int_to_bytes(fake_msg.buf, IGNORE_VALUE);
		if (!obc_can.buffer_outgoing(fake_msg))
		{
			Serial.println("Outgoing buffer failed to queue msg");
		};
	};
}

void setup(void) {
	Serial.println("Setting up");
	can1.begin();
	can1.setBaudRate(250000);
	seed_random(analogRead(0));
	#ifdef SIMULATE_OBC
	Serial.println("Simulating OBC");
	timer = StensTimer::getInstance();
	timer->setStaticCallback(timer_callback);
	timer->setInterval(FAKE_OBC_MESSAGE, FAKE_OBC_MESSAGE_MS);
	#endif
	Serial.println("Setup completed");
}

void loop() {
	if (!can.incoming_empty())
	{
		#ifdef PERIPHERAL_BRAKE
		static CAN_message_t msg;
		can.read(msg);
		brake_ns::canbus_payload_to_brake(brake_s_, msg, can);
		#endif
	}
	if (!can.outgoing_empty()) {
		Serial.print("Peripheral outgoing buffers: ");
		Serial.println(can.n_outgoing_buffers());
		can.send();
	}
	#ifdef SIMULATE_OBC
	timer->run();
	// This keeps bricking the Teensy...
	obc_can.send();
	#endif
}
