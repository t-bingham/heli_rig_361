/*
 * ENCE 361 Group Helicopter Project
 * Helicopter buttons header file
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Original code from P.J.Bones UCECE
 * Last modified: 03.06.2018
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdint.h>
#include <stdbool.h>

// Constants
enum butNames {UP = 0, DOWN, LEFT, RIGHT, MODE, NUM_BUTS};
enum butStates {RELEASED = 0, PUSHED, NO_CHANGE};

// UP button (active high)
#define UP_PERIPHERAL    SYSCTL_PERIPH_GPIOE
#define UP_BASE          GPIO_PORTE_BASE
#define UP_PIN           GPIO_PIN_0
#define UP_NORMAL        false

// DOWN button (active high)
#define DOWN_PERIPHERAL  SYSCTL_PERIPH_GPIOD
#define DOWN_BASE        GPIO_PORTD_BASE
#define DOWN_PIN         GPIO_PIN_2
#define DOWN_NORMAL      false

// LEFT button (active low)
#define LEFT_PERIPHERAL  SYSCTL_PERIPH_GPIOF
#define LEFT_BASE        GPIO_PORTF_BASE
#define LEFT_PIN         GPIO_PIN_4
#define LEFT_NORMAL      true

// RIGHT button (active low)
#define RIGHT_PERIPHERAL SYSCTL_PERIPH_GPIOF
#define RIGHT_BASE       GPIO_PORTF_BASE
#define RIGHT_PIN        GPIO_PIN_0
#define RIGHT_NORMAL     true

// MODE switch (high == flying)
#define MODE_PERIPHERAL  SYSCTL_PERIPH_GPIOA
#define MODE_BASE        GPIO_PORTA_BASE
#define MODE_PIN         GPIO_PIN_7
#define MODE_NORMAL      false

#define NUM_BUT_POLLS 3
// Debounce algorithm: A state machine is associated with each button.
// A state change occurs only after NUM_BUT_POLLS consecutive polls have
// read the pin in the opposite condition, before the state changes and
// a flag is set.  Set NUM_BUT_POLLS according to the polling rate.

// initButtons: Initialise the variables associated with the set of buttons
// defined by the constants above.
void initButtons(void);

// updateButtons: Function designed to be called regularly. It polls all
// buttons once and updates variables associated with the buttons if
// necessary.  It is efficient enough to be part of an ISR, e.g. from
// a SysTick interrupt.
void updateButtons(void);

// checkButton: Function returns the new button state if the button state
// (PUSHED or RELEASED) has changed since the last call, otherwise returns
// NO_CHANGE.  The argument butName should be one of constants in the
// enumeration butStates, excluding 'NUM_BUTS'. Safe under interrupt.
uint8_t checkButton(uint8_t butName);

#endif /*BUTTONS_H_*/
