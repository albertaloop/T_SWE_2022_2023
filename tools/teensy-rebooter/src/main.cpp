#include <Arduino.h>

uint8_t ASCII_OFFSET = 48; // ie. 0 + 48 = 48 which is Zero in ASCII. 9 + 48 = 57 which is Nine in ASCII
uint8_t PROGRAMMING_PIN_MIN = 0; // Zero in ASCII
uint8_t PROGRAMMING_PIN_MAX = 9; // Nine in ASCII
uint8_t PROGRAMMING_TIME_SEC = 2;

uint8_t programming_pin = 0;

uint8_t get_low_pin()
{
  int b = 0;
  while (b < (PROGRAMMING_PIN_MIN + ASCII_OFFSET) || b > (PROGRAMMING_PIN_MAX + ASCII_OFFSET))
  {
    if (Serial.available()) {
      b = Serial.read();
    }
  }
  return b - ASCII_OFFSET;
}

void setup() {
  for (int i = PROGRAMMING_PIN_MIN; i <= PROGRAMMING_PIN_MAX; ++i)
    pinMode(i, OUTPUT_OPENDRAIN); // Open Drain because we only care about driving to ground
                                  // https://open4tech.com/open-drain-output-vs-push-pull-output/
  while(!Serial); // wait for Serial port
  Serial.println("Welcome to the Teensy Rebooter program. Press any key to start");
}

void loop() {
  if (Serial.available()) { // press any key to restart program
    Serial.println("Welcome to the Teensy Rebooter program.");
    Serial.print("Tell us which pin to set low [1-10]: ");
    programming_pin = get_low_pin();
    Serial.println(programming_pin);
    Serial.print("Pulling pin ");
    Serial.print(programming_pin);
      Serial.print(" low for ");
      Serial.print(PROGRAMMING_TIME_SEC);
        Serial.println(" seconds.");
    digitalWrite(programming_pin, LOW);
    delay(PROGRAMMING_TIME_SEC*1000);
    digitalWrite(programming_pin, HIGH);
    Serial.println("Finished programming. Press any key to retry");
  }
}