#include <cstring>

#include "utils.hpp"

void int_to_bytes(int i, uint8_t *a) {
  // this is NOT working
  a = static_cast<uint8_t *>(static_cast<void *>(&i));
}

void int_to_bytes2(int i, uint8_t *a) {
  a[0] = (i & 0x000000ff);
  a[1] = (i & 0x0000ff00) >> 8;
  a[2] = (i & 0x00ff0000) >> 16;
  a[3] = (i & 0xff000000) >> 24;
}

void int_of_bytes(int i, uint8_t *a) { memcpy(&i, a, sizeof(i)); }
