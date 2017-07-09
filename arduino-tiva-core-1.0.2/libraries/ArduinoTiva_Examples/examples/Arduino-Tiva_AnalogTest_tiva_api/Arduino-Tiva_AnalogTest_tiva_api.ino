#include "driverlib/adc.h"

// Time keeping for periodic process execution
const uint32_t delta_t = 100000; // 100 ms
uint32_t prevtime_us = 0;
uint32_t currtime_us = 0;

#define CHANS 8
uint32_t ui32Values[CHANS];
uint8_t channel = 0;

void setup() {
  // delay allows VCOM port to settle   
  delay(2000);
    
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

    
    //
    // Enable the ADC0 module.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
  
    //
    // Wait for the ADC0 module to be ready.
    //
    while(!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) { }  
  
    //
    // Enable the first sample sequencer to capture the value of channel 0 through 7 when
    // the processor trigger occurs.
    //
    ROM_ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    for (int n=0; n<CHANS; n++)
    {
      ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, n, n);
      if (n==CHANS-1)
        ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, n, ADC_CTL_IE | ADC_CTL_END | n);
    }
    ROM_ADCSequenceEnable(ADC0_BASE, 0);
    
    //
    // Trigger the sample sequence.
    //
    ROM_ADCProcessorTrigger(ADC0_BASE, 0);
    
    //
    // Wait until the sample sequence has completed.
    //
    while(!ROM_ADCIntStatus(ADC0_BASE, 0, false)) { }
    
    //
    // Read the values from the ADC.
    // Place results starting at address of ui32Values[0]
    //
    ROM_ADCSequenceDataGet(ADC0_BASE, 0, &ui32Values[0]);

    // Send value over usb serial port
    for (int n=0; n<CHANS; n++)
    {
      USBSerial.print(ui32Values[n]);
      USBSerial.print(", ");
      ui32Values[n] = 0;
    }
    USBSerial.println();
  }
}
