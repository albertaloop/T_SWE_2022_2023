#include <Arduino.h>
#include <utility/imumaths.h>
#include <Adafruit_BNO055.h>
#include <avr/interrupt.h>
#include <avr/io.h>

bool flag = false;
int step = 0;

Adafruit_BNO055 bno = Adafruit_BNO055(55);

// When the Timer1 counter reaches the same value as Output Compare register 1, the TIMER1_COMPA_vect triggers this interrupt subroutine:
ISR(TIMER1_COMPA_vect){
  flag = true;
}
 
void setup(void) 
{
  Serial.begin(9600);  
  /* Initialise the sensor */
  if( !bno.begin() )
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);

  bno.setExtCrystalUse(true);

  TIMSK1 |= (1<<OCIE1A); // Enables timer interrupts

  OCR1A = 12499; // Sets the timer to compare match 
  TCCR1A |= (1<<WGM13) | (1 <<WGM12); // Enables CTC mode (Clear Timer on Compare)
  TCCR1B |= (1<<CS11) | (1<<CS10); // Set pre-scaler of 256

  sei(); // enable interrupts

}
 
void loop(void) 
{
  if(flag){
    flag = false;
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    Serial.print('Z');
    Serial.print( accel.x() );
    Serial.print(' ');


    Serial.println(step);
    step += 1;
    if (step > 9){
      step = 0;
    }
  }

  // OCR1A is what the clock counts up to before triggering the interrupt 
  // Since a pre-scaler of 256 is used, every the Timer1 counter increments every 256 clock cycles
  // 256*12499 = 3,199,744‬ clock cycles to trigger a compare match event
  // My uC clock speed was at 16,000,000 cycles/s
  // So 3,199,744 [cycles] / 16,000,000 [cycles/s] = 0.2 [s] = 200 [ms]

}