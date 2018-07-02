/*
 * ENCE 361 Group Helicopter Project
 * Helicopter pwm code
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Original code from P.J.Bones UCECE
 * Last modified: 03.06.2018
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "utils/ustdlib.h"
#include "stdlib.h"
#include "pwm.h"

#define PWM_RATE_HZ          200
#define PWM_MAIN_START_DUTY  2
#define PWM_TAIL_START_DUTY  2
#define PWM_DIVIDER_CODE     SYSCTL_PWMDIV_1
#define PWM_DIVIDER          1

#define PWM_MAIN_BASE        PWM0_BASE
#define PWM_MAIN_GEN         PWM_GEN_3
#define PWM_MAIN_OUTNUM      PWM_OUT_7
#define PWM_MAIN_OUTBIT      PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM  SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE   GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN    GPIO_PIN_5

#define PWM_TAIL_BASE        PWM1_BASE
#define PWM_TAIL_GEN         PWM_GEN_2
#define PWM_TAIL_OUTNUM      PWM_OUT_5
#define PWM_TAIL_OUTBIT      PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWM  SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE   GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN    GPIO_PIN_1

static uint32_t mainDutyCycle;
static int32_t tailDutyCycle;

/**
 * Initialise the two PWM generators that drive the main rotor and the tail
 * rotor.
 */
void initPWM(void)
{
    mainDutyCycle = PWM_MAIN_START_DUTY;
    tailDutyCycle = PWM_TAIL_START_DUTY;

    // Init main rotor PWM
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);
    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);
    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
            PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    setMainRotorDuty(mainDutyCycle);
    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);

    // Init tail rotor PWM
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);
    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);
    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN,
            PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    setTailRotorDuty(tailDutyCycle);
    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);

    // Enable
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
}

/**
 * Sets the duty cycle for the main rotor PWM generator
 */
void setMainRotorDuty(uint32_t duty)
{
    mainDutyCycle = duty;
    uint32_t period = SysCtlClockGet() / PWM_DIVIDER / PWM_RATE_HZ;
    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, period * duty / 100);
}

/**
 * Sets the duty cycle for the tail rotor PWM generator
 */
void setTailRotorDuty(uint32_t duty)
{
    tailDutyCycle = duty;
    uint32_t period = SysCtlClockGet() / PWM_DIVIDER / PWM_RATE_HZ;
    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM, period * duty / 100);
}

/**
 * Returns the current duty cycle of the main rotor PWM generator
 */
uint32_t getMainRotorDuty(void)
{
    return mainDutyCycle;
}

/**
 * Returns the current duty cycle of the tail rotor PWM generator
 */
uint32_t getTailRotorDuty(void)
{
    return tailDutyCycle;
}
