int sensePin = A9;
int sensorInput;
double temp;
const int min_room_temp = 10;
const int max_room_temp = 30;

void setup() { Serial.begin(9600); }

void loop() {
  // put your main code here, to run repeatedly:
  sensorInput = analogRead(sensePin);
  temp = (double)sensorInput / 1024;
  temp *= 5;
  temp -= 0.5;
  temp *= 100;

  // TODO: Check if temp is recorded in C
  test_room_temp(temp);
  delay(1000);
}

// Roomtemp test returns true if test is data fetched is with roomtemp, false
// otherwise
bool test_room_temp(double temp) {
  // reads for 10 data points
  for (size_t i = 0; i < 10; i++) {
    if (!(min_room_temp < temp < max_room_temp)) {
      Serial.println("Test Room Temperature failed!");
      return false;
    }
  }
  // true if roomtemp test passed
}
