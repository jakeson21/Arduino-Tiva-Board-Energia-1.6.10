#ifndef USBVIRTUALSERIAL_H_
#define USBVIRTUALSERIAL_H_

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

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

//*****************************************************************************
//
// The size of the transmit and receive buffers used for the redirected UART.
// This number should be a power of 2 for best performance.  256 is chosen
// pretty much at random though the buffer should be at least twice the size of
// a maxmum-sized USB packet.
//
//*****************************************************************************
#define UART_BUFFER_SIZE 512

extern uint32_t RxHandler(void *pvCBData, uint32_t ui32Event,
                          uint32_t ui32MsgValue, void *pvMsgData);
extern uint32_t TxHandler(void *pvi32CBData, uint32_t ui32Event,
                          uint32_t ui32MsgValue, void *pvMsgData);

extern tUSBBuffer g_sTxBuffer;
extern tUSBBuffer g_sRxBuffer;
extern tUSBDCDCDevice g_sCDCDevice;
extern uint8_t g_pui8USBTxBuffer[];
extern uint8_t g_pui8USBRxBuffer[];

//*****************************************************************************
//
// Flag indicating whether or not we are currently sending a Break condition.
//
//*****************************************************************************
extern bool g_bSendingBreak;

//*****************************************************************************
//
// Global flag indicating that a USB configuration has been set.
//
//*****************************************************************************
extern bool g_bUSBConfigured;

extern uint32_t g_ui32VCPRxCount;
extern uint32_t g_ui32VCPTxCount;

extern uint32_t g_ui32Flags;
extern char *g_pcStatus;

//*****************************************************************************
//
// Defines callback used to check host requested serial LineCoding used to
// initiate switching to DFU for boot loader.
//
//*****************************************************************************
typedef void (* tVCOMCheckInitBootloaderCallback)(tLineCoding *psLineCoding);
extern tVCOMCheckInitBootloaderCallback pfnCheckInitBootloader;
#define BOOTLOADER_INIT_SERIAL_RATE 1200

//*****************************************************************************
//
// Default line coding settings for the redirected UART.
//
//*****************************************************************************
#define DEFAULT_BIT_RATE        115200
#define DEFAULT_UART_CONFIG     (UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | \
                                 UART_CONFIG_STOP_ONE)


//*****************************************************************************
//
// Flags used to pass commands from interrupt context to the main loop.
//
//*****************************************************************************
#define COMMAND_PACKET_RECEIVED 0x00000001
#define COMMAND_STATUS_UPDATE   0x00000002

//*****************************************************************************
//
// Internal function prototypes.
//
//*****************************************************************************
extern void USBUARTPrimeTransmit(void);
extern void SetControlLineState(uint16_t ui16State);
extern bool SetLineCoding(tLineCoding *psLineCoding);
extern void GetLineCoding(tLineCoding *psLineCoding);
extern void SendBreak(bool bSend);
extern void InitUSBVCOM(tVCOMCheckInitBootloaderCallback ptr);

extern uint32_t g_ui32Flags;
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* USBVIRTUALSERIAL_H_ */
