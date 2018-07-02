/*
 * ENCE 361 Group Helicopter Project
 * Helicopter yaw code
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
#include "yaw.h"

#define YAW_NUM_QEI_SLOTS        112
#define YAW_CONVERSION_CONSTANT  (90.0 / YAW_NUM_QEI_SLOTS)

#define YAW_PERIPHERAL           SYSCTL_PERIPH_GPIOB
#define YAW_BASE                 GPIO_PORTB_BASE
#define YAW_CHANNEL_A            GPIO_PIN_0
#define YAW_CHANNEL_B            GPIO_PIN_1
#define YAW_PINS                 (GPIO_PIN_0 | GPIO_PIN_1)
#define YAW_INT                  INT_GPIOB

#define YAW_REFERENCE_PERIPHERAL SYSCTL_PERIPH_GPIOC
#define YAW_REFERENCE_BASE       GPIO_PORTC_BASE
#define YAW_REFERENCE_PIN        GPIO_PIN_4
#define YAW_REFERENCE_INT        INT_GPIOC

static volatile bool isReferenceInitialised;
static volatile int32_t yaw;
static int32_t visibleYaw;
static uint16_t currentState;
static uint16_t lastState;
static bool channelAState;
static bool channelBState;

/**
 * Sets the reference position for the yaw. This interupt is not needed after
 * its first call, so is immediately disabled.
 */
static void yawReferenceIntHandler(void)
{
    GPIOIntClear(YAW_REFERENCE_BASE, YAW_REFERENCE_PIN);
    IntDisable(YAW_REFERENCE_INT);
    isReferenceInitialised = true;
    yaw = 0;
}

/**
 * Updates the yaw according to its last state and current state
 */
static void yawCalc()
{
    if ((lastState == 1 && currentState == 2) ||
            (lastState == 2 && currentState == 3) ||
            (lastState == 3 && currentState == 4) ||
            (lastState == 4 && currentState == 1)) {
        yaw++;
    } else if (currentState == lastState) {
    } else {
        yaw--;
    }
}

/**
 * QEI state machine that updates the yaw
 *
 * Note that this state machine only works if called on any change of state for
 * channel A or B
 */
static void yawIntHandler(void)
{
    GPIOIntClear(YAW_BASE, YAW_PINS);

    channelAState = GPIOPinRead(YAW_BASE, YAW_CHANNEL_A);
    channelBState = GPIOPinRead(YAW_BASE, YAW_CHANNEL_B);
    lastState = currentState;

    if (!channelAState) {
        if (channelBState) {
            currentState = 2;
        } else {
            currentState = 1;
        }
    } else {
        if (channelBState) {
            currentState = 3;
        } else {
            currentState = 4;
        }
    }

    yawCalc();
}

/**
 * Initialise the yaw module
 */
void initYaw(void)
{
    SysCtlPeripheralEnable(YAW_REFERENCE_PERIPHERAL);
    while (!SysCtlPeripheralReady(YAW_REFERENCE_PERIPHERAL))
    {
    }
    GPIOPinTypeGPIOInput(YAW_REFERENCE_BASE, YAW_REFERENCE_PIN);
    IntRegister(YAW_REFERENCE_INT, yawReferenceIntHandler);
    GPIOIntTypeSet(YAW_REFERENCE_BASE, YAW_REFERENCE_PIN, GPIO_FALLING_EDGE);
    GPIOIntEnable(YAW_REFERENCE_BASE, YAW_REFERENCE_PIN);
    IntEnable(YAW_REFERENCE_INT);
    bool atReferencePosition = !GPIOPinRead(YAW_REFERENCE_BASE, YAW_REFERENCE_PIN);
    if (atReferencePosition) {
        isReferenceInitialised = true;
        /*
         * Once the reference position has been intialised, the yawReference
         * interrupt is no longer needed
         */
        IntDisable(YAW_REFERENCE_INT);
    }

    SysCtlPeripheralEnable(YAW_PERIPHERAL);
    while (!SysCtlPeripheralReady(YAW_PERIPHERAL))
    {
    }
    GPIOPinTypeGPIOInput(YAW_BASE, YAW_PINS);
    IntRegister(YAW_INT, yawIntHandler);
    GPIOIntTypeSet(YAW_BASE, YAW_PINS, GPIO_BOTH_EDGES);
    GPIOIntEnable(YAW_BASE, YAW_PINS);
    IntEnable(YAW_INT);
    yaw = 0;
}

/**
 * Updates the yaw to the last instance of yaw
 */
void updateYaw(void)
{
    visibleYaw = yaw;
}

/**
 * Returns the yaw in degrees
 */
int32_t getYaw(void)
{
    return visibleYaw * YAW_CONVERSION_CONSTANT;
}

/**
 * Returns whether the reference position has been initialised
 */
bool checkReferenceInitialised(void)
{
    return isReferenceInitialised;
}
