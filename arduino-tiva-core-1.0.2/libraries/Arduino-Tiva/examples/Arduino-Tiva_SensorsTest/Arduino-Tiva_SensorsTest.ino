// Author: Jacob Miller
// 2017-07-04
//
// Example program which demonstrates the use of DigitalRead and DigitialWrite
// Also demonstrates the use of USBSerial.print()
//

// Time keeping for periodic process execution
const uint32_t delta_t = 1000; // 10 ms
uint32_t prevtime_us = 0;
uint32_t currtime_us = 0;

// Using two DF Robot crash sensors: https://www.dfrobot.com/wiki/index.php/Crash_Sensor_(SKU:_SEN0138)
// Physical switches wired as:
// SW1 (LED 'on' when switch closed. Switch output is pulled high. Goes low on contact.)
// Data => D4
// Vcc  => D3
// Gnd  => D2
//
// SW2 (LED 'on' when switch closed. Switch output is pulled high. Goes low on contact.)
// Data => D7
// Vcc  => D6
// Gnd  => D5
//
const int SW1 = 4;
const int SW2 = 7;

bool a = false;
bool b = false;

void setup() {
    // put your setup code here, to run once:
    prevtime_us = micros();

    pinMode(SW1, INPUT);
    pinMode(SW1-1, OUTPUT); // Vcc
    pinMode(SW1-2, OUTPUT); // Gnd
    digitalWrite(SW1-1, HIGH);
    digitalWrite(SW1-2, LOW);

    pinMode(SW2, INPUT);
    pinMode(SW2-1, OUTPUT); // Vcc
    pinMode(SW2-2, OUTPUT); // Gnd
    digitalWrite(SW2-1, HIGH);
    digitalWrite(SW2-2, LOW);

    USBSerial.println("Crash sensor test program\n");
}

void loop() {
  // put your main code here, to run repeatedly:

  // Only process every so often
  currtime_us = micros();
  if (currtime_us - prevtime_us >= delta_t)
  {
    prevtime_us = currtime_us;

    bool update_a = false;
    bool update_b = false;

    // Read state of each switch. If change is detected, set flag.
    update_a |= a != digitalRead(SW1) ? true : false;
    if (update_a) { a ^= 1; }
    update_b |= b != digitalRead(SW2) ? true : false;
    if (update_b) { b ^= 1; }

    // If change occured, print current states
    if (update_a || update_b)
    {
      USBSerial.print(currtime_us);
      USBSerial.print("\tA=");
      USBSerial.print(a);
      USBSerial.print(" B=");
      USBSerial.print(b);
      USBSerial.println();
    }
  }

}
