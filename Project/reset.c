/*
 * ENCE 361 Group Helicopter Project
 * Helicopter reset code
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Last modified: 03.06.2018
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"
#include "reset.h"

#define RESET_PERIPHERAL SYSCTL_PERIPH_GPIOA
#define RESET_BASE       GPIO_PORTA_BASE
#define RESET_PIN        GPIO_PIN_6
#define RESET_INT        INT_GPIOA

/**
 * Calls SysCtlReset() API function
 */
static void resetIntHandler(void)
{
    GPIOIntClear(RESET_BASE, RESET_PIN);
    SysCtlReset();
}

/**
 * Initialise the reset module
 */
void initReset(void)
{
    SysCtlPeripheralEnable(RESET_PERIPHERAL);
    while (!SysCtlPeripheralReady(RESET_PERIPHERAL))
    {
    }
    GPIOPinTypeGPIOInput(RESET_BASE, RESET_PIN);
    IntRegister(RESET_INT, resetIntHandler);
    GPIOIntTypeSet(RESET_BASE, RESET_PIN, GPIO_FALLING_EDGE);
    GPIOIntEnable(RESET_BASE, RESET_PIN);
    IntEnable(RESET_INT);
}
