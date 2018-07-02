/*
 * ENCE 361 Group Helicopter Project
 * Helicopter main file
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Last modified: 03.06.2018
 */

#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "driverlib/timer.h"
#include "buttons.h"
#include "stdlib.h"
#include "altitude.h"
#include "yaw.h"
#include "display.h"
#include "reset.h"
#include "pwm.h"
#include "uart.h"
#include "pid.h"

#define BUTTON_POLL_RATE_HZ 100
#define STATUS_LENGTH       200

/**
 * Initialise the clock
 */
void initClock(void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
            SYSCTL_XTAL_16MHZ);
}

/**
 * Interrupt handler for updating the buttons
 */
void buttonUpdateTimerIntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    updateButtons();
}

/**
 * Initialise the timer used to poll the buttons
 */
void initButtonUpdateTimer(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
    {
    }
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / BUTTON_POLL_RATE_HZ);
    TimerIntRegister(TIMER0_BASE, TIMER_A, buttonUpdateTimerIntHandler);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

/**
 * Updates the PID's target altitude and yaw depending on button presses
 */
void updatePIDTargets(void)
{
    if (checkButton(UP) == PUSHED) {
        increaseAltitudeTarget(10);
    } else if (checkButton(DOWN) == PUSHED) {
        increaseAltitudeTarget(-10);
    } else if (checkButton(LEFT) == PUSHED) {
        increaseYawTarget(-15);
    } else if (checkButton(RIGHT) == PUSHED) {
        increaseYawTarget(15);
    }

}

/**
 * Interrupt            | Requested On | Handler     | File
 * ==============================================================
 * Yaw calculation      | BOTH_EDGES   | PB0 and PB1 | yaw.c
 * Yaw reference        | FALLING_EDGE | PC4         | yaw.c
 * Reset                | FALLING_EDGE | PA6         | reset.c
 * Button Update        | 100 Hz       | Timer A0    | main.c
 * Altitude ADC Trigger | 120 Hz       | SysTick     | altitude.c
 */
int main(void)
{
    initClock();
    initButtons();
    initAltitude();
    initYaw();
    initPWM();
    initReset();
    initButtonUpdateTimer();
    initUART();
    initDisplay();
    IntMasterEnable();

    uint32_t dataOutputCount = 0;
    uint32_t mode = 0;
    while (1) {

        updateAltitude();
        updateYaw();

        // SETUP
        if (mode == 0) {
            setTailRotorDuty(8);
            if (checkReferenceInitialised()) {
                setTailRotorDuty(0);
                mode = 2;
            }
        }

        // FLYING
        if (mode == 1) {
            updatePIDTargets();
            updatePID();
            if (checkButton(MODE) == RELEASED){
                mode = 2;
            }
        }

        // LANDING
        if (mode == 2) {
            setYawTarget(0);
            if ((getAltitudeTarget() != 0) && (getAltitudeTarget() != 5)) {
                // Entering first stage of landing, find yaw = 0.
                setAltitudeTarget(5);
                updatePID();
            } else if ((getYaw() == 0) && (getAltitudeTarget() == 5)) {
                // Yaw = 0, completed first Stage of landing. Entering second stage, find altitude = 0%.
                setAltitudeTarget(0);
                updatePID();
            } else if ((getYaw() == 0) && (getAltitudePercentage() == 0) && (getAltitudeTarget() == 0)) {
                // At altitude 0%, completed second Stage of landing. Cutting motors.
                setTailRotorDuty(0);
                setMainRotorDuty(0);
                mode = 3;
            } else {
                // Didn't meet requirements to enter next stage of landing, maintain current targets.
                updatePID();
            }
        }

        // LANDED
        if (mode == 3) {
            if (checkButton(MODE) == PUSHED){
                mode = 1;
            }
        }


        if (dataOutputCount % 255 == 0) {
            updateDisplay();

            char statusStr[STATUS_LENGTH];
            usnprintf(statusStr,
                      STATUS_LENGTH,
                      "-------------\nAlt:%d [%d]\n\Yaw:%d [%d]\nMain:%d Tail:%d\nSetup %d\n\r",
                      getAltitudePercentage(),
                      getAltitudeTarget(),
                      getYaw(),
                      getYawTarget(),
                      getMainRotorDuty(),
                      getTailRotorDuty(),
                      mode);
            sendUART(statusStr);
            dataOutputCount = 0;
        } else {
            dataOutputCount++;
        }
    }
}
