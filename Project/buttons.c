/*
 * ENCE 361 Group Helicopter Project
 * Helicopter buttons code
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Original code from P.J.Bones UCECE
 * Last modified: 03.06.2018
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"
#include "buttons.h"

static bool but_state[NUM_BUTS]; 
static uint8_t but_count[NUM_BUTS];
static bool but_flag[NUM_BUTS];
static bool but_normal[NUM_BUTS];

/**
 * Initialise the variables associated with the set of buttons defined by the
 * constants in the buttons2.h header file
 */
void initButtons(void)
{
    int i;

    // UP button (active HIGH)
    SysCtlPeripheralEnable(UP_PERIPHERAL);
    GPIOPinTypeGPIOInput(UP_BASE, UP_PIN);
    GPIOPadConfigSet(UP_BASE, UP_PIN, GPIO_STRENGTH_2MA,
            GPIO_PIN_TYPE_STD_WPD);
    but_normal[UP] = UP_NORMAL;

    // DOWN button (active HIGH)
    SysCtlPeripheralEnable(DOWN_PERIPHERAL);
    GPIOPinTypeGPIOInput(DOWN_BASE, DOWN_PIN);
    GPIOPadConfigSet(DOWN_BASE, DOWN_PIN, GPIO_STRENGTH_2MA,
            GPIO_PIN_TYPE_STD_WPD);
    but_normal[DOWN] = DOWN_NORMAL;

    // LEFT button (active LOW)
    SysCtlPeripheralEnable(LEFT_PERIPHERAL);
    GPIOPinTypeGPIOInput(LEFT_BASE, LEFT_PIN);
    GPIOPadConfigSet(LEFT_BASE, LEFT_PIN, GPIO_STRENGTH_2MA,
            GPIO_PIN_TYPE_STD_WPU);
    but_normal[LEFT] = LEFT_NORMAL;

    // RIGHT button (active LOW)
    // Note that PF0 is one of a handful of GPIO pins that need to be
    // "unlocked" before they can be reconfigured.  This also requires
    // #include "inc/tm4c123gh6pm.h"
    SysCtlPeripheralEnable(RIGHT_PERIPHERAL);
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R |= GPIO_PIN_0;
    GPIO_PORTF_LOCK_R = GPIO_LOCK_M;
    GPIOPinTypeGPIOInput(RIGHT_BASE, RIGHT_PIN);
    GPIOPadConfigSet(RIGHT_BASE, RIGHT_PIN, GPIO_STRENGTH_2MA,
            GPIO_PIN_TYPE_STD_WPU);
    but_normal[RIGHT] = RIGHT_NORMAL;

    // MODE switch (high == flying)
    SysCtlPeripheralEnable(MODE_PERIPHERAL);
    GPIOPinTypeGPIOInput(MODE_BASE, MODE_PIN);
    GPIOPadConfigSet(MODE_BASE, MODE_PIN, GPIO_STRENGTH_2MA,
            GPIO_PIN_TYPE_STD_WPU);
    but_normal[MODE] = MODE_NORMAL;

    for (i = 0; i < NUM_BUTS; i++) {
        but_state[i] = but_normal[i];
        but_count[i] = 0;
        but_flag[i] = false;
    }
}

/**
 * Function designed to be called regularly. It polls all buttons once and
 * updates variables associated with the buttons if necessary.  It is efficient
 * enough to be part of an ISR, e.g. from a SysTick interrupt. Debounce
 * algorithm: A state machine is associated with each button. A state change
 * occurs only after NUM_BUT_POLLS consecutive polls have read the pin in the
 * opposite condition, before the state changes and a flag is set.  Set
 * NUM_BUT_POLLS according to the polling rate.
 */
void updateButtons(void)
{
    bool but_value[NUM_BUTS];
    int i;

    but_value[UP] = (GPIOPinRead(UP_BASE, UP_PIN) == UP_PIN);
    but_value[DOWN] = (GPIOPinRead(DOWN_BASE, DOWN_PIN) == DOWN_PIN);
    but_value[LEFT] = (GPIOPinRead(LEFT_BASE, LEFT_PIN) == LEFT_PIN);
    but_value[RIGHT] = (GPIOPinRead(RIGHT_BASE, RIGHT_PIN) == RIGHT_PIN);
    but_value[MODE] = (GPIOPinRead(MODE_BASE, MODE_PIN) == MODE_PIN);

    for (i = 0; i < NUM_BUTS; i++) {
        if (but_value[i] != but_state[i]) {
            but_count[i]++;
            if (but_count[i] >= NUM_BUT_POLLS) {
                but_state[i] = but_value[i];
                but_flag[i] = true; // Reset by call to checkButton()
                but_count[i] = 0;
            }
        } else {
            but_count[i] = 0;
        }
    }
}

/**
 * Function returns the new button logical state if the button logical state
 * (PUSHED or RELEASED) has changed since the last call, otherwise returns
 * NO_CHANGE
 */
uint8_t checkButton(uint8_t butName)
{
    if (but_flag[butName]) {
        but_flag[butName] = false;
        if (but_state[butName] == but_normal[butName]) {
            return RELEASED;
        } else {
            return PUSHED;
        }
    }
    return NO_CHANGE;
}
