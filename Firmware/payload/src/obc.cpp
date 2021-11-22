#include "Arduino.h"

#include "FlexCAN_T4.h"

#include "utils.hpp"

namespace obc {
/* On Board Computer */
enum TRANSITION {
  START  ,
  STOP   ,
};
struct command {
  TRANSITION transition;
  uint8_t* value;
};
enum STATE {
  SAFE,         // Default state. Pod is on, not moving and safe for approach
  READY,        // Ready to launch - not accelerating
  LAUNCHING,    // Commanding propulsion for highest velocity
  COASTING,     // Moving at a constant velocity
  BRAKING,      // Decelerating with brake
  CRAWLING,     // Commading propulsion for not highest velocity (??)
  FAULT,        // Something went wrong
};
}