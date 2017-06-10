/*
  ************************************************************************
  *	USBVirtualSerial.h
  *
  *	Arduino core files for ARM Cortex-M4F: Tiva-C and Stellaris
  *		Copyright (c) 2017 Jacob Miller. All right reserved.
  *
  *
  ***********************************************************************

  Derived from:
  HardwareSerial.cpp - Hardware serial library for Arduino
  Copyright (c) 2012 Robert Wessels.    All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef USBVIRTUALSERIAL_H_
#define USBVIRTUALSERIAL_H_

#include <inttypes.h>
#include "Stream.h"

#define USB_SERIAL_BUFFER_SIZE     128
#define BOOTLOADER_INIT_SERIAL_RATE 1200

// Default line coding settings for the redirected UART.
#define DEFAULT_BIT_RATE        115200
#define DEFAULT_UART_CONFIG     (UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | \
                                 UART_CONFIG_STOP_ONE)
// Flags used to pass commands from interrupt context to the main loop.
#define COMMAND_PACKET_RECEIVED 0x00000001
#define COMMAND_STATUS_UPDATE   0x00000002

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_nvic.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/usb.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"

#include "usblib/usblib.h"
#include "usblib/usbcdc.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdcdc.h"

// Defines callback used to check host requested serial LineCoding used to
// initiate switching to DFU for boot loader.
typedef void (* tVCOMCheckInitBootloaderCallback)(tLineCoding *psLineCoding);

class USBVirtualSerial : public Stream
{
    public:
		USBVirtualSerial(void);
		// USBVirtualSerial(unsigned long);
		void begin(tVCOMCheckInitBootloaderCallback);
		void setBufferSize(unsigned long, unsigned long);
		void setModule(unsigned long);
		void setPins(unsigned long);
		void end(void);
		virtual int available(void);
		virtual int peek(void);
		virtual int read(void);
		virtual void flush(void);
		virtual size_t write(uint8_t c);
		operator bool();
		using Print::write; // pull in write(str) and write(buf, size) from Print

        // Handlers
        uint32_t RxHandler(void *pvCBData, uint32_t ui32Event,
                           uint32_t ui32MsgValue, void *pvMsgData);
        uint32_t TxHandler(void *pvi32CBData, uint32_t ui32Event,
                           uint32_t ui32MsgValue, void *pvMsgData);
        uint32_t ControlHandler(void *pvCBData, uint32_t ui32Event,
                                uint32_t ui32MsgValue, void *pvMsgData);

        uint32_t bytesSent(void)     { return ui32VCPTxCount; }
        uint32_t bytesReceived(void) { return ui32VCPRxCount; }
        tLineCoding getConfiguredLineCoding(void) const { return sConfiguredLineCoding; }
        uint32_t ctrlHandlerCalled;
        uint32_t ctrlHandlerCount;

    private:
        //*****************************************************************************
        //
        // The size of the transmit and receive buffers used for the redirected UART.
        // This number should be a power of 2 for best performance.  256 is chosen
        // pretty much at random though the buffer should be at least twice the size of
        // a maxmum-sized USB packet.
        //
        //*****************************************************************************
        tUSBDCDCDevice sCDCDevice;
        tUSBBuffer sTxBuffer;
        tUSBBuffer sRxBuffer;
        uint8_t pui8USBTxBuffer[USB_SERIAL_BUFFER_SIZE];
        uint8_t pui8USBRxBuffer[USB_SERIAL_BUFFER_SIZE];
        // Flag indicating whether or not we are currently sending a Break condition.
        bool bSendingBreak;
        // Flag indicating that a USB configuration has been set.
        bool bUSBConfigured;
        // Count of bytes sent and received
        uint32_t ui32VCPRxCount;
        uint32_t ui32VCPTxCount;
        uint32_t ui32Flags;
        char *pcStatus;
        tVCOMCheckInitBootloaderCallback pfnCheckInitBootloader;
        tLineCoding sConfiguredLineCoding;

        void flushAll(void);
		void primeReceive();

        // Internal function prototypes.
        void SetControlLineState(uint16_t ui16State);
        bool SetLineCoding(tLineCoding *psLineCoding);
        void GetLineCoding(tLineCoding *psLineCoding);
        void SendBreak(bool bSend);
        void InitUSBVCOM(tVCOMCheckInitBootloaderCallback ptr);
};

extern USBVirtualSerial USBSerial;

#endif /* USBVIRTUALSERIAL_H_ */
