/* Teensy Rebooter Code
 *
 * Released under MIT license
 * Author mike.d.bardwell@pm.me
 */

#include <Arduino.h>

uint8_t ASCII_OFFSET = 48; // ie. 0 + 48 = 48 which is Zero in ASCII. 9 + 48 = 57 which is Nine in ASCII
uint8_t PROGRAMMING_PIN_MIN = 0; // Zero in ASCII
uint8_t PROGRAMMING_PIN_MAX = 9; // Nine in ASCII
uint8_t PROGRAMMING_TIME_SEC = 2;

uint8_t programming_pin = 0;

/* Stalls the program until the user inputs a valid pin number.
 * Valid pin numbers are between 0-9.
 */
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
  {
    /* We have to set all of the pins to INPUT, or else the pins
     * will go low when the rebooter Teensy starts, putting the
     * Teensies we want to program into HalfKey state prematurely.
     */
    pinMode(i, INPUT_PULLUP);
  }
  while(!Serial); // wait for Serial port
  Serial.println("Welcome to the Teensy Rebooter program. Press any key to start");
}

void loop() {
  if (Serial.available()) {
    Serial.println("Welcome to the Teensy Rebooter program.");
    Serial.print("Tell us which pin to set low [");
      Serial.print(PROGRAMMING_PIN_MIN);
        Serial.print("-");
          Serial.print(PROGRAMMING_PIN_MAX);
            Serial.print("]: ");
    programming_pin = get_low_pin();
    Serial.println(programming_pin);
    Serial.print("Pulling pin ");
    Serial.print(programming_pin);
      Serial.print(" low for ");
      Serial.print(PROGRAMMING_TIME_SEC);
        Serial.println(" seconds.");
    /* Here we get creative. We set the programming pin to OUTPUT.
     * Then, we set it LOW as is required to put the Teensy we want
     * to program into HalfKey mode. We then wait a few seconds and then
     * return the pin to INPUT mode. The Teensy we want to program will
     * stay in HalfKey mode until we program it or power cycle the unit.
     */
    pinMode(programming_pin, OUTPUT);
    digitalWrite(programming_pin, LOW);
    delay(PROGRAMMING_TIME_SEC*1000);
    digitalWrite(programming_pin, HIGH);
    pinMode(programming_pin, INPUT_PULLUP);
    Serial.println("Finished programming. Press any key to retry");
  }
}