#include <Energia.h>

#if defined(PART_TM4C129XNCZAD)
#include "inc/tm4c129xnczad.h"
#elif defined(PART_TM4C1294NCPDT)
#include "inc/tm4c1294ncpdt.h"
#elif defined(PART_TM4C123GH6PM) || defined(PART_LM4F120H5QR)
#include "inc/tm4c123gh6pm.h"
#else
#error "**** No PART defined or unsupported PART ****"
#endif

#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/eeprom.h"

// USB VCOM Bootloader Handler

//*****************************************************************************
//
// Switches to DFU mode
//
//*****************************************************************************
bool g_bInitiateBootloaderFlag = false;

void initiateDFUupdate()
{
    // pause for 1/10 th of a second to allow dfuprog to get serial state
    ROM_SysCtlDelay(SysCtlClockGet() / (3*10));

    //
    // Terminate the USB device and detach from the bus.
    //
    USBDCDCTerm(0);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4);

    //
    // Disable all interrupts.
    //
    ROM_IntMasterDisable();

    //
    // Disable SysTick and its interrupt.
    //
    ROM_SysTickIntDisable();    // Not using SysTick()
    ROM_SysTickDisable();

    //
    // Disable all processor interrupts.  Instead of disabling them one at a
    // time, a direct write to NVIC is done to disable all peripheral
    // interrupts.
    //
    HWREG(NVIC_DIS0) = 0xffffffff;
    HWREG(NVIC_DIS1) = 0xffffffff;
    HWREG(NVIC_DIS2) = 0xffffffff;
    HWREG(NVIC_DIS3) = 0xffffffff;
    HWREG(NVIC_DIS4) = 0xffffffff;

    //
    // Enable and reset the USB peripheral.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    ROM_SysCtlPeripheralReset(SYSCTL_PERIPH_USB0);
    ROM_SysCtlUSBPLLEnable();

    //
    // Wait for about a second.
    //
    ROM_SysCtlDelay(SysCtlClockGet() / 3);

    //
    // Re-enable interrupts at the NVIC level.
    //
    ROM_IntMasterEnable();

    // NOTE: include string table in this array directly, not as address to another array.
    uint8_t pui8USBBootROMInfo[] = {
        0xbe, 0x1c, // TI VID
        0xff, 0x00, // Tiva DFU PID
        0x01, 0x00, // Device Release(BCD 0x0001)
        USB_CONF_ATTR_BUS_PWR, // 0x00=Bus Powered, 0xC0=Self powered using no bus power
        250/2, // 0x4B=150mA of Bus power, 2mA steps

        4, // One language
        USB_DTYPE_STRING,
        USBShort(USB_LANG_EN_US), // English
        // "Texas Instruments"
        (17 * 2) + 2, // Size of Manufacturer String.
        USB_DTYPE_STRING,
        'T', 0, 'e', 0, 'x', 0, 'a', 0, 's', 0, ' ', 0, 'I', 0, 'n', 0,
        's', 0, 't', 0, 'r', 0, 'u', 0, 'm', 0, 'e', 0, 'n', 0, 't', 0,
        's', 0,
        // "Device Firmware Upgrade"
        (23 * 2) + 2, // Size of Product String.
        USB_DTYPE_STRING,
        'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0, ' ', 0, 'F', 0,
        'i', 0, 'r', 0, 'm', 0, 'w', 0, 'a', 0, 'r', 0, 'e', 0, ' ', 0,
        'U', 0, 'p', 0, 'g', 0, 'r', 0, 'a', 0, 'd', 0, 'e', 0,
        // "1.0" (serial number)
        (3 * 2) + 2, // Size of Serial Number.
        USB_DTYPE_STRING,
        '1', 0, '.', 0, '0', 0
        };

    //
    // Call the USB boot loader.
    //
    ROM_UpdateUSB(pui8USBBootROMInfo);
}

void CheckForBootloaderRequest(tLineCoding *psLineCoding)
{
    g_bInitiateBootloaderFlag = false;
    USBSerial.print("Connected at ");
    USBSerial.println(psLineCoding->ui32Rate);
    if (psLineCoding->ui32Rate == BOOTLOADER_INIT_SERIAL_RATE)
    {
        USBSerial.println("Bootloader time");
        g_bInitiateBootloaderFlag = true;
    }
}

#ifdef __cplusplus
extern "C" {

void _init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	if(ROM_EEPROMInit() == EEPROM_INIT_ERROR) {
		if(ROM_EEPROMInit() != EEPROM_INIT_ERROR)
			EEPROMMassErase();
	}

	timerInit();
    
    ROM_FPUEnable();

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
#ifdef TARGET_IS_SNOWFLAKE_RA0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOR);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOS);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOT);
#endif

	//Unlock and commit NMI pins PD7 and PF0
	// HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x4C4F434B;
	// HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x1;
	// HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x4C4F434B;
	// HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;

	// Deep Sleep mode init
	// DSLP clock = PIOSC / 16 = 1MHz
	// Note: Couldn't find the define constants for SysCtlDeepSleepPowerSet in the driverlib.
	//
	#ifdef TARGET_IS_BLIZZARD_RB1
	ROM_SysCtlDeepSleepClockSet(SYSCTL_DSLP_DIV_16 | SYSCTL_DSLP_OSC_INT);
	SysCtlDeepSleepPowerSet(0x21);  // FLASHPM = LOW_POWER_MODE, SRAMPM = STANDBY_MODE
	SysCtlLDODeepSleepSet(SYSCTL_LDO_1_00V);  // Going lower tends to be very flaky and cause continual resets
						  // particularly when measuring MCU current.
	#endif
	//
	#ifdef TARGET_IS_SNOWFLAKE_RA0
	ROM_SysCtlDeepSleepClockConfigSet(16, SYSCTL_DSLP_OSC_INT);
	SysCtlDeepSleepPowerSet(0x121);  // TSPD, FLASHPM = LOW_POWER_MODE, SRAMPM = STANDBY_MODE
	#endif
	//
    

} /* void _init(void) */

} /* extern "C" */
#endif

int main(void)
{
    USBSerial.begin(&CheckForBootloaderRequest);

	setup();

	for (;;) {
        if (g_bInitiateBootloaderFlag) { initiateDFUupdate(); }
        loop();
        if (serialEventRun) serialEventRun();
	}
}
