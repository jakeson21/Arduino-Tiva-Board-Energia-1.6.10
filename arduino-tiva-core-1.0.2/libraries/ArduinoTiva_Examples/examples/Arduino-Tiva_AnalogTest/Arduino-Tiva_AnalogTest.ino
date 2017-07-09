// Jacob Miller 
// 2017-07-04
//
// Also demonstrates the use of USBSerial.print()
//


// Time keeping for periodic process execution
#define NUMANALOG 12
const uint32_t delta_t = 100000; // 100 ms
uint32_t prevtime_us = 0;
uint32_t currtime_us = 0;
int analog_pins[NUMANALOG] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11};

uint32_t analogVal;

void setup() {
  // delay allows VCOM port to settle   
  delay(2000);

  // take first time reading
  prevtime_us = micros();

  // print welcome message
  USBSerial.println("AnalogRead test program");
  USBSerial.println("Values range from 0 to 4096 (for 0V to 3.3V)\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Only process every so often
  currtime_us = micros();
  if (currtime_us - prevtime_us >= delta_t)
  {    
    prevtime_us = currtime_us;

    // Read each analog pin and print result
    for (int n=0; n<NUMANALOG; n++)
    {
      analogVal = analogRead(analog_pins[n]);      
      USBSerial.print(analogVal);
      USBSerial.print(", ");
    }
    USBSerial.println();    
  }
}
