// Author: Jacob Miller
// Date: 5/13/2017
// Computes time (micro-seconds) to perform certain math functions
// Prints results to COM port in Tab-Delimited format
//

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include <driverlib/sysctl.h>

uint32_t count = 0;
uint32_t time_prev_ms = 0;
uint32_t period_ms = 1000;
tLineCoding lc;

void setup() {
    delay(2000);
    Serial.begin(57600);
    Serial.println("Hardware Serial");
    USBSerial.println("USB Serial");    
}

void loop() {
    long time_c =  millis();
    if (time_c - time_prev_ms >= period_ms)
    {
        time_prev_ms = time_c;
        count++;
        lc = USBSerial.getConfiguredLineCoding();
    
        Serial.print("CtrlHndl status = ");
        Serial.println(USBSerial.ctrlHandlerCalled, HEX);
        Serial.print("CtrlHndl count = ");
        Serial.println(USBSerial.ctrlHandlerCount);
        Serial.print("USB bytes sent = ");
        Serial.println(USBSerial.bytesSent());
        Serial.print("USB bytes received = ");
        Serial.println(USBSerial.bytesReceived());
        Serial.print("VCOM Rate = ");
        Serial.println(lc.ui32Rate);    
        Serial.println(count);
    
        USBSerial.print("CtrlHndl status = ");
        USBSerial.println(USBSerial.ctrlHandlerCalled, HEX);
        USBSerial.print("CtrlHndl count = ");
        USBSerial.println(USBSerial.ctrlHandlerCount);
        USBSerial.print("USB bytes sent = ");
        USBSerial.println(USBSerial.bytesSent());
        USBSerial.print("USB bytes received = ");
        USBSerial.println(USBSerial.bytesReceived());
        USBSerial.print("VCOM Rate = ");
        USBSerial.println(lc.ui32Rate);    
        USBSerial.println(count);
    }

    // send data only when you receive data:
    bool gotSerial = false;
    while (USBSerial.available()) {
        gotSerial = true;
        // read the incoming byte:
        char incomingByte = USBSerial.read();
        // say what you got:
        USBSerial.print(incomingByte);
    }
    if (gotSerial) { USBSerial.println(""); }
}

