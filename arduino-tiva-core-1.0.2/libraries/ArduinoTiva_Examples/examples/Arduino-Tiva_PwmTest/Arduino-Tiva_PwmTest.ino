// Jacob Miller 
// 2017-07-08
//
// Demonstrates using Servo library.
// Also demonstrates the use of USBSerial.parseInt()
//

#include <Servo.h>
Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
int pos = 0;    // variable to store the servo position
enum DIR {REV=-1, FWD=1} direction;    // tracks direction. 1=fwd, -1=rev
const uint32_t delta_t = 500000; // 100 ms
uint32_t prevtime_us = 0;
uint32_t currtime_us = 0;
int current_servo_pin = 2;

void setup() 
{
  // delay allows VCOM port to settle
  delay(2000);
  myservo.attach(current_servo_pin);  // attaches the servo on pin 1
  direction = FWD;

  // take first time reading
  prevtime_us = micros();

  // print welcome message
  USBSerial.println("Servo (PWM) test program");
  USBSerial.setTimeout(50);

  USBSerial.print("Servo on pin ");
  USBSerial.println(current_servo_pin);
  USBSerial.print("Enter a number 2 to 34 to change to that digital pin");

}

void loop()
{
    // Only process every so often
  currtime_us = micros();
  if (currtime_us - prevtime_us >= delta_t)
  {    
    prevtime_us = currtime_us;
    
    USBSerial.print("Next position is ");
    USBSerial.println(pos);
    myservo.write(pos);              // tell servo to go to position in variable 'pos'

    if (direction == FWD) 
    { 
      pos += FWD*15;
      if (pos>=180) 
      { 
        USBSerial.println("Reversing");
        direction = REV; 
        pos = 180;
      }
    }
    else
    {
      pos += REV*15;
      if (pos<=0) 
      { 
        USBSerial.println("Reversing");
        direction = FWD; 
        pos = 0;
      }
    }
  }

  while (USBSerial.available()) {
    long val = USBSerial.parseInt();

    if (val == 0) { USBSerial.println("Error: turn off line endings in serial console"); }
    else
    {
      //myservo.detach();
      myservo.attach(val);
      // say what you got:
      current_servo_pin = val;
      USBSerial.print("Servo on pin ");
      USBSerial.println(current_servo_pin);
    }
  }
}
