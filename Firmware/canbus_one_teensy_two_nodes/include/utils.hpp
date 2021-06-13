#include <cstdint>
#include <FlexCAN_T4.h>

void int_to_bytes(int i, uint8_t* a);
void int_to_bytes2(int i, uint8_t* a);
void int_of_bytes(int i, uint8_t* a);

void canSniff(const CAN_message_t &msg);