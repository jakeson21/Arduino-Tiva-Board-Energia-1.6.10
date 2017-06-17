#include <USBVirtualSerial.h>

//*****************************************************************************
//
// The languages supported by this device.
//
//*****************************************************************************
static const uint8_t g_pui8LangDescriptor[] =
{
    4,
    USB_DTYPE_STRING,
    USBShort(USB_LANG_EN_US)
};

//*****************************************************************************
//
// The manufacturer string.
//
//*****************************************************************************
static const uint8_t g_pui8ManufacturerString[] =
{
    (17 + 1) * 2,
    USB_DTYPE_STRING,
    'T', 0, 'e', 0, 'x', 0, 'a', 0, 's', 0, ' ', 0, 'I', 0, 'n', 0, 's', 0,
    't', 0, 'r', 0, 'u', 0, 'm', 0, 'e', 0, 'n', 0, 't', 0, 's', 0,
};

//*****************************************************************************
//
// The product string.
//
//*****************************************************************************
static const uint8_t g_pui8ProductString[] =
{
    2 + (16 * 2),
    USB_DTYPE_STRING,
    'V', 0, 'i', 0, 'r', 0, 't', 0, 'u', 0, 'a', 0, 'l', 0, ' ', 0,
    'C', 0, 'O', 0, 'M', 0, ' ', 0, 'P', 0, 'o', 0, 'r', 0, 't', 0
};

//*****************************************************************************
//
// The serial number string.
//
//*****************************************************************************
static const uint8_t g_pui8SerialNumberString[] =
{
    2 + (3 * 2),
    USB_DTYPE_STRING,
    '1', 0, '.', 0, '0', 0
};

//*****************************************************************************
//
// The control interface description string.
//
//*****************************************************************************
static const uint8_t g_pui8ControlInterfaceString[] =
{
    2 + (21 * 2),
    USB_DTYPE_STRING,
    'A', 0, 'C', 0, 'M', 0, ' ', 0, 'C', 0, 'o', 0, 'n', 0, 't', 0,
    'r', 0, 'o', 0, 'l', 0, ' ', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0,
    'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0
};

//*****************************************************************************
//
// The configuration description string.
//
//*****************************************************************************
static const uint8_t g_pui8ConfigString[] =
{
    2 + (25 * 2),
    USB_DTYPE_STRING,
    'B', 0, 'u', 0, 's', 0, ' ', 0, 'P', 0, 'o', 0, 'w', 0,
    'e', 0, 'r', 0, 'e', 0, 'd', 0, ' ', 0, 'C', 0, 'o', 0, 'n', 0,
    'f', 0, 'i', 0, 'g', 0, 'u', 0, 'r', 0, 'a', 0, 't', 0, 'i', 0,
    'o', 0, 'n', 0
};

//*****************************************************************************
//
// The descriptor string table.
//
//*****************************************************************************
static const uint8_t * const g_ppui8StringDescriptors[] =
{
    g_pui8LangDescriptor,
    g_pui8ManufacturerString,
    g_pui8ProductString,
    g_pui8SerialNumberString,
    g_pui8ControlInterfaceString,
    g_pui8ConfigString
};

#define NUM_STRING_DESCRIPTORS (sizeof(g_ppui8StringDescriptors) /            \
                                sizeof(uint8_t *))


static uint32_t RxHandlerProxy(void *pvCBData, uint32_t ui32Event,
            uint32_t ui32MsgValue, void *pvMsgData)
{
    USBSerial.RxHandler(pvCBData, ui32Event, ui32MsgValue, pvMsgData);
}

static uint32_t TxHandlerProxy(void *pvi32CBData, uint32_t ui32Event,
            uint32_t ui32MsgValue, void *pvMsgData)
{
    USBSerial.TxHandler(pvi32CBData, ui32Event, ui32MsgValue, pvMsgData);
}

static uint32_t CtrlHandlerProxy(void *pvCBData, uint32_t ui32Event,
              uint32_t ui32MsgValue, void *pvMsgData)
{
    USBSerial.ControlHandler(pvCBData, ui32Event, ui32MsgValue, pvMsgData);
}

USBVirtualSerial::USBVirtualSerial()
{
    //*****************************************************************************
    //
    // The CDC device initialization and customization structures. In this case,
    // we are using USBBuffers between the CDC device class driver and the
    // application code. The function pointers and callback data values are set
    // to insert a buffer in each of the data channels, transmit and receive.
    //
    // With the buffer in place, the CDC channel callback is set to the relevant
    // channel function and the callback data is set to point to the channel
    // instance data. The buffer, in turn, has its callback set to the application
    // function and the callback data set to our CDC instance structure.
    //
    //*****************************************************************************
    sCDCDevice = 
    {
        USB_VID_TI_1CBE,
        USB_PID_SERIAL,
        250/2,
        USB_CONF_ATTR_BUS_PWR,
        &CtrlHandlerProxy,
        (void *)&sCDCDevice,
        USBBufferEventCallback,
        (void *)&sRxBuffer,
        USBBufferEventCallback,
        (void *)&sTxBuffer,
        g_ppui8StringDescriptors,
        NUM_STRING_DESCRIPTORS
    };

    // Receive buffer (from the USB perspective).
    sRxBuffer =
    {
        false,                          // This is a receive buffer.
        &RxHandlerProxy,                // pfnCallback
        (void *)&sCDCDevice,            // Callback data is our device pointer.
        USBDCDCPacketRead,              // pfnTransfer
        USBDCDCRxPacketAvailable,       // pfnAvailable
        (void *)&sCDCDevice,            // pvHandle
        pui8USBRxBuffer,                // pui8Buffer
        USB_SERIAL_BUFFER_SIZE,         // ui32BufferSize
    };

    // Transmit buffer (from the USB perspective).
    sTxBuffer =
    {
        true,                           // This is a transmit buffer.
        &TxHandlerProxy,                // pfnCallback
        (void *)&sCDCDevice,            // Callback data is our device pointer.
        USBDCDCPacketWrite,             // pfnTransfer
        USBDCDCTxPacketAvailable,       // pfnAvailable
        (void *)&sCDCDevice,            // pvHandle
        pui8USBTxBuffer,                // pui8Buffer
        USB_SERIAL_BUFFER_SIZE,         // ui32BufferSize
    };

    // Flag indicating whether or not we are currently sending a Break condition.
    bool bSendingBreak = false;

    // Global flag indicating that a USB configuration has been set.
    bUSBConfigured = false;

    ui32VCPRxCount = 0;
    ui32VCPTxCount = 0;
    ui32Flags = 0;
    pcStatus = 0;
    pfnCheckInitBootloader = 0;
}

// Nothing to do
void USBVirtualSerial::begin(tVCOMCheckInitBootloaderCallback pBootloaderCallback)
{
    ctrlHandlerCalled = 0;
    sConfiguredLineCoding = {0,0,0,0};
    ctrlHandlerCount = 0;

    InitUSBVCOM(pBootloaderCallback);
}

void USBVirtualSerial::setBufferSize(unsigned long txsize, unsigned long rxsize)
{
    // // Allocate TX & RX buffers
    // if (pui8USBTxBuffer != (uint8_t *)0xFFFFFFFF)  // Catch attempts to re-init this Serial instance by freeing old buffer first
        // free(pui8USBTxBuffer);
    // if (pui8USBRxBuffer != (uint8_t *)0xFFFFFFFF)  // Catch attempts to re-init this Serial instance by freeing old buffer first
        // free(pui8USBRxBuffer);
    // pui8USBTxBuffer = (uint8_t *) malloc(txsize);
    // pui8USBRxBuffer = (uint8_t *) malloc(rxsize);

    // // Reset USB buffers to new locations
    // sTxBuffer.pui8Buffer = pui8USBTxBuffer;
    // sRxBuffer.pui8Buffer = pui8USBRxBuffer;
}

// Nothing to do
void USBVirtualSerial::setModule(unsigned long module) { }
// Nothing to do
void USBVirtualSerial::setPins(unsigned long pins) { }
// Nothing to do. Don't actually disable USB.
void USBVirtualSerial::end(void) { }

int USBVirtualSerial::available(void)
{
    return USBBufferDataAvailable((tUSBBuffer *)&sRxBuffer);
}

int USBVirtualSerial::peek(void)
{
    //
    // Wait for a character to be received.
    //
    if(available() == 0)
    {
        return -1;
        //
        // Block waiting for a character to be received (if the buffer is
        // currently empty).
        //
    }

    // Read a character from the buffer without advancing pointer
    uint8_t ui8Char;
    uint32_t ui32Read;
    // Get read index in sRxBuffer
    tUSBRingBufObject psRingBuf;
    USBBufferInfoGet((tUSBBuffer *)&sRxBuffer, &psRingBuf);
    // Read directly from buffer. Do not increment read index.
    ui8Char = psRingBuf.pui8Buf[(psRingBuf.ui32ReadIndex + 1) % psRingBuf.ui32Size];
    // Return the character to the caller.
    return(ui8Char);
}

int USBVirtualSerial::read(void)
{
    // Wait for a character to be received.
    if(available() == 0)
    {
        return -1;
        // Block waiting for a character to be received (if the buffer is
        // currently empty).
    }

    // Read a character from the buffer.
    uint8_t ui8Char;
    uint32_t ui32Read;
    ui32Read = USBBufferRead((tUSBBuffer *)&sRxBuffer, &ui8Char, 1);
    ui32VCPRxCount++;
    // Return the character to the caller.
    return(ui8Char);
}

void USBVirtualSerial::flush(void)
{
    USBBufferFlush((tUSBBuffer *)&sRxBuffer);
    USBBufferFlush((tUSBBuffer *)&sTxBuffer);
}

size_t USBVirtualSerial::write(uint8_t c)
{
    // unsigned int numTransmit = 0;
    //
    // Check for valid arguments.
    //
    // ASSERT(c != 0);

    USBBufferWrite((tUSBBuffer *)&sTxBuffer, (uint8_t *)&c, 1);

    return 1;
}

USBVirtualSerial::operator bool()
{
	return true;
}


// Private Methods //////////////////////////////////////////////////////////////
void
USBVirtualSerial::flushAll(void)
{
    flush();
}

void
USBVirtualSerial::primeReceive()
{
    uint32_t ui32Read = 0;
    uint8_t ui8Char = 0;

    //
    // If we are currently sending a break condition, don't receive any
    // more data. We will resume transmission once the break is turned off.
    //
    if(bSendingBreak)
    {
        return;
    }

    //
    // If there is space in the UART FIFO, try to read some characters
    // from the receive buffer to fill it again.
    //
    while(USBBufferDataAvailable((tUSBBuffer *)&sRxBuffer))
    {
        // For now, just let calls to USBSerial.read() pull data out of buffer.
        // But this needs to be here too.

        //
        // Get a character from the buffer.
        //
        //ui32Read = USBBufferRead((tUSBBuffer *)&sRxBuffer, &ui8Char, 1);

        //
        // Did we get a character?
        //
        if(ui32Read)
        {
            // Also Echo back to the USB VCOM Port
            // USBBufferWrite((tUSBBuffer *)&sTxBuffer, (uint8_t *)&ui8Char, 1);

            //
            // Update our count of bytes received via the UART.
            //
            //ui32VCPRxCount++;
        }
        else
        {
            //
            // We ran out of characters so exit the function.
            //
            return;
        }
    }
}

//*****************************************************************************
//
// Set the state of the RS232 RTS and DTR signals.
//
//*****************************************************************************
void
USBVirtualSerial::SetControlLineState(uint16_t ui16State)
{
    //
    // TODO: If configured with GPIOs controlling the handshake lines,
    // set them appropriately depending upon the flags passed in the wValue
    // field of the request structure passed.
    //
}

//*****************************************************************************
//
// Set the communication parameters to use on the UART.
//
//*****************************************************************************
bool
USBVirtualSerial::SetLineCoding(tLineCoding *psLineCoding)
{
    uint32_t ui32Config;
    bool bRetcode;

    //
    // Assume everything is OK until we detect any problem.
    //
    bRetcode = true;

    sConfiguredLineCoding.ui32Rate = psLineCoding->ui32Rate;
    sConfiguredLineCoding.ui8Stop = psLineCoding->ui8Stop;
    sConfiguredLineCoding.ui8Parity = psLineCoding->ui8Parity;
    sConfiguredLineCoding.ui8Databits = psLineCoding->ui8Databits;

    // Callback to check for need to enter bootloader
    if (pfnCheckInitBootloader != 0)
    {
        pfnCheckInitBootloader(psLineCoding);
    }

    //
    // Word length.  For invalid values, the default is to set 8 bits per
    // character and return an error.
    //
    switch(psLineCoding->ui8Databits)
    {
        case 5:
        {
            ui32Config = UART_CONFIG_WLEN_5;
            break;
        }

        case 6:
        {
            ui32Config = UART_CONFIG_WLEN_6;
            break;
        }

        case 7:
        {
            ui32Config = UART_CONFIG_WLEN_7;
            break;
        }

        case 8:
        {
            ui32Config = UART_CONFIG_WLEN_8;
            break;
        }

        default:
        {
            ui32Config = UART_CONFIG_WLEN_8;
            bRetcode = false;
            break;
        }
    }

    //
    // Parity.  For any invalid values, we set no parity and return an error.
    //
    switch(psLineCoding->ui8Parity)
    {
        case USB_CDC_PARITY_NONE:
        {
            ui32Config |= UART_CONFIG_PAR_NONE;
            break;
        }

        case USB_CDC_PARITY_ODD:
        {
            ui32Config |= UART_CONFIG_PAR_ODD;
            break;
        }

        case USB_CDC_PARITY_EVEN:
        {
            ui32Config |= UART_CONFIG_PAR_EVEN;
            break;
        }

        case USB_CDC_PARITY_MARK:
        {
            ui32Config |= UART_CONFIG_PAR_ONE;
            break;
        }

        case USB_CDC_PARITY_SPACE:
        {
            ui32Config |= UART_CONFIG_PAR_ZERO;
            break;
        }

        default:
        {
            ui32Config |= UART_CONFIG_PAR_NONE;
            bRetcode = false;
            break;
        }
    }

    //
    // Stop bits.  Our hardware only supports 1 or 2 stop bits whereas CDC
    // allows the host to select 1.5 stop bits.  If passed 1.5 (or any other
    // invalid or unsupported value of ui8Stop, we set up for 1 stop bit but
    // return an error in case the caller needs to Stall or otherwise report
    // this back to the host.
    //
    switch(psLineCoding->ui8Stop)
    {
        //
        // One stop bit requested.
        //
        case USB_CDC_STOP_BITS_1:
        {
            ui32Config |= UART_CONFIG_STOP_ONE;
            break;
        }

        //
        // Two stop bits requested.
        //
        case USB_CDC_STOP_BITS_2:
        {
            ui32Config |= UART_CONFIG_STOP_TWO;
            break;
        }

        //
        // Other cases are either invalid values of ui8Stop or values that we
        // cannot support so set 1 stop bit but return an error.
        //
        default:
        {
            ui32Config |= UART_CONFIG_STOP_ONE;
            bRetcode = false;
            break;
        }
    }
    
    if (!bRetcode) { sConfiguredLineCoding = {0,0,0,0}; }

    //
    // Let the caller know if we had a problem or not.
    //
    return(bRetcode);
}

//*****************************************************************************
//
// Get the communication parameters in use on the UART.
//
//*****************************************************************************
void
USBVirtualSerial::GetLineCoding(tLineCoding *psLineCoding)
{
    //uint32_t ui32Config = UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE;

    //
    // Get the current default line coding
    //
    psLineCoding->ui32Rate = sConfiguredLineCoding.ui32Rate;
    psLineCoding->ui8Databits = sConfiguredLineCoding.ui8Databits;
    psLineCoding->ui8Parity = sConfiguredLineCoding.ui8Parity;
    psLineCoding->ui8Stop = sConfiguredLineCoding.ui8Stop;
    /*
    //
    // Translate the configuration word length field into the format expected
    // by the host.
    //
    switch(ui32Config & UART_CONFIG_WLEN_MASK)
    {
        case UART_CONFIG_WLEN_8:
        {
            psLineCoding->ui8Databits = 8;
            break;
        }

        case UART_CONFIG_WLEN_7:
        {
            psLineCoding->ui8Databits = 7;
            break;
        }

        case UART_CONFIG_WLEN_6:
        {
            psLineCoding->ui8Databits = 6;
            break;
        }

        case UART_CONFIG_WLEN_5:
        {
            psLineCoding->ui8Databits = 5;
            break;
        }
    }

    //
    // Translate the configuration parity field into the format expected
    // by the host.
    //
    switch(ui32Config & UART_CONFIG_PAR_MASK)
    {
        case UART_CONFIG_PAR_NONE:
        {
            psLineCoding->ui8Parity = USB_CDC_PARITY_NONE;
            break;
        }

        case UART_CONFIG_PAR_ODD:
        {
            psLineCoding->ui8Parity = USB_CDC_PARITY_ODD;
            break;
        }

        case UART_CONFIG_PAR_EVEN:
        {
            psLineCoding->ui8Parity = USB_CDC_PARITY_EVEN;
            break;
        }

        case UART_CONFIG_PAR_ONE:
        {
            psLineCoding->ui8Parity = USB_CDC_PARITY_MARK;
            break;
        }

        case UART_CONFIG_PAR_ZERO:
        {
            psLineCoding->ui8Parity = USB_CDC_PARITY_SPACE;
            break;
        }
    }

    //
    // Translate the configuration stop bits field into the format expected
    // by the host.
    //
    switch(ui32Config & UART_CONFIG_STOP_MASK)
    {
        case UART_CONFIG_STOP_ONE:
        {
            psLineCoding->ui8Stop = USB_CDC_STOP_BITS_1;
            break;
        }

        case UART_CONFIG_STOP_TWO:
        {
            psLineCoding->ui8Stop = USB_CDC_STOP_BITS_2;
            break;
        }
    }
    */
}

//*****************************************************************************
//
// This function sets or clears a break condition on the redirected UART RX
// line.  A break is started when the function is called with \e bSend set to
// \b true and persists until the function is called again with \e bSend set
// to \b false.
//
//*****************************************************************************
void
USBVirtualSerial::SendBreak(bool bSend)
{

}
//    //
//    // Are we being asked to start or stop the break condition?
//    //
//    if(!bSend)
//    {
//        //
//        // Remove the break condition on the line.
//        //
//        MAP_UARTBreakCtl(USB_UART_BASE, false);
//        g_bSendingBreak = false;
//    }
//    else
//    {
//        //
//        // Start sending a break condition on the line.
//        //
//        MAP_UARTBreakCtl(USB_UART_BASE, true);
//        g_bSendingBreak = true;
//    }
//}

//*****************************************************************************
//
// Handles CDC driver notifications related to control and setup of the device.
//
// \param pvCBData is the client-supplied callback pointer for this channel.
// \param ui32Event identifies the event we are being notified about.
// \param ui32MsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the CDC driver to perform control-related
// operations on behalf of the USB host.  These functions include setting
// and querying the serial communication parameters, setting handshake line
// states and sending break conditions.
//
// \return The return value is event-specific.
//
//*****************************************************************************
uint32_t
USBVirtualSerial::ControlHandler(void *pvCBData, uint32_t ui32Event,
               uint32_t ui32MsgValue, void *pvMsgData)
{
    uint32_t ui32IntsOff;

    //
    // Which event are we being asked to process?
    //
    ctrlHandlerCalled = ui32Event;
    ctrlHandlerCount++;
    
    switch(ui32Event)
    {
        //
        // We are connected to a host and communication is now possible.
        //
        case USB_EVENT_CONNECTED:
            bUSBConfigured = true;

            //
            // Flush our buffers.
            //
            USBBufferFlush(&sTxBuffer);
            USBBufferFlush(&sRxBuffer);

            //
            // Tell the main loop to update the display.
            //
            ui32IntsOff = MAP_IntMasterDisable();
            pcStatus = "Connected";
            ui32Flags |= COMMAND_STATUS_UPDATE;
            if(!ui32IntsOff)
            {
                MAP_IntMasterEnable();
            }
            break;

        //
        // The host has disconnected.
        //
        case USB_EVENT_DISCONNECTED:
            bUSBConfigured = false;
            ui32IntsOff = MAP_IntMasterDisable();
            pcStatus = "Disconnected";
            ui32Flags |= COMMAND_STATUS_UPDATE;
            if(!ui32IntsOff)
            {
                MAP_IntMasterEnable();
            }
            break;

        //
        // Return the current serial communication parameters.
        //
        case USBD_CDC_EVENT_GET_LINE_CODING:
            GetLineCoding((tLineCoding*)pvMsgData);
            break;

        //
        // Set the current serial communication parameters.
        //
        case USBD_CDC_EVENT_SET_LINE_CODING:
            SetLineCoding((tLineCoding*)pvMsgData);
            break;

        //
        // Set the current serial communication parameters.
        //
        case USBD_CDC_EVENT_SET_CONTROL_LINE_STATE:
            SetControlLineState((uint16_t)ui32MsgValue);
            break;

        //
        // Send a break condition on the serial line.
        //
        case USBD_CDC_EVENT_SEND_BREAK:
            SendBreak(true);
            break;

        //
        // Clear the break condition on the serial line.
        //
        case USBD_CDC_EVENT_CLEAR_BREAK:
            SendBreak(false);
            break;

        //
        // Ignore SUSPEND and RESUME for now.
        //
        case USB_EVENT_SUSPEND:
        case USB_EVENT_RESUME:
            break;

        //
        // We don't expect to receive any other events.  Ignore any that show
        // up in a release build or hang in a debug build.
        //
        default:
#ifdef DEBUG
            while(1);
#else
            break;
#endif

    }
    return(0);
}

//*****************************************************************************
//
// Handles CDC driver notifications related to the transmit channel (data to
// the USB host).
//
// \param ui32CBData is the client-supplied callback pointer for this channel.
// \param ui32Event identifies the event we are being notified about.
// \param ui32MsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the CDC driver to notify us of any events
// related to operation of the transmit data channel (the IN channel carrying
// data to the USB host).
//
// \return The return value is event-specific.
//
//*****************************************************************************
uint32_t
USBVirtualSerial::TxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue, void *pvMsgData)
{
    //
    // Which event have we been sent?
    //
    switch(ui32Event)
    {
        case USB_EVENT_TX_COMPLETE:
            //
            // Since we are using the USBBuffer, we don't need to do anything
            // here. Update Tx byte count.
            //
            ui32VCPTxCount += ui32MsgValue;
            break;

        //
        // We don't expect to receive any other events.  Ignore any that show
        // up in a release build or hang in a debug build.
        //
        default:
#ifdef DEBUG
            while(1);
#else
            break;
#endif

    }
    return(0);
}

//*****************************************************************************
//
// Handles CDC driver notifications related to the receive channel (data from
// the USB host).
//
// \param ui32CBData is the client-supplied callback data value for this channel.
// \param ui32Event identifies the event we are being notified about.
// \param ui32MsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the CDC driver to notify us of any events
// related to operation of the receive data channel (the OUT channel carrying
// data from the USB host).
//
// \return The return value is event-specific.
//
//*****************************************************************************
uint32_t
USBVirtualSerial::RxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue, void *pvMsgData)
{
//    uint32_t ui32Count;

    //
    // Which event are we being sent?
    //
    switch(ui32Event)
    {
        //
        // A new packet has been received.
        //
        case USB_EVENT_RX_AVAILABLE:
        {
            //
            // Feed some characters into the UART TX FIFO and enable the
            // interrupt so we are told when there is more space.
            //
            primeReceive();
            break;
        }

        //
        // We are being asked how much unprocessed data we have still to
        // process. We return 0 if the UART is currently idle or 1 if it is
        // in the process of transmitting something. The actual number of
        // bytes in the UART FIFO is not important here, merely whether or
        // not everything previously sent to us has been transmitted.
        //
        case USB_EVENT_DATA_REMAINING:
        {
            //
            // Get the number of bytes in the buffer and add 1 if some data
            // still has to clear the transmitter.
            //
            return(0);
        }

        //
        // We are being asked to provide a buffer into which the next packet
        // can be read. We do not support this mode of receiving data so let
        // the driver know by returning 0. The CDC driver should not be sending
        // this message but this is included just for illustration and
        // completeness.
        //
        case USB_EVENT_REQUEST_BUFFER:
        {
            return(0);
        }

        //
        // We don't expect to receive any other events.  Ignore any that show
        // up in a release build or hang in a debug build.
        //
        default:
#ifdef DEBUG
            while(1);
#else
            break;
#endif
    }
    return(0);
}

//*****************************************************************************
//
//
//
//
//*****************************************************************************
void USBVirtualSerial::InitUSBVCOM(tVCOMCheckInitBootloaderCallback ptr)
{
    bSendingBreak = false;
    ui32Flags = 0;
    ui32VCPTxCount = 0;
    ui32VCPRxCount = 0;
    pfnCheckInitBootloader = ptr;
    
    //
    // Configure the required pins for USB operation.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    MAP_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4);

    //
    // Not configured initially.
    //
    bUSBConfigured = false;

    //
    // Initialize the transmit and receive buffers.
    //
    USBBufferInit(&sTxBuffer);
    USBBufferInit(&sRxBuffer);

    //
    // Set the USB stack mode to Device mode with VBUS monitoring.
    //
    USBStackModeSet(0, eUSBModeForceDevice, 0);

    //
    // Pass our device information to the USB library and place the device
    // on the bus.
    //
    USBDCDCInit(0, &sCDCDevice);
}

// Global declaration
USBVirtualSerial USBSerial;
