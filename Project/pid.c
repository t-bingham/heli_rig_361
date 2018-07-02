/*
 * ENCE 361 Group Helicopter Project
 * Helicopter pid code
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
#include "pwm.h"
#include "altitude.h"
#include "pid.h"

#define ALTITUDE_KP         0.2
#define ALTITUDE_KI         0.012
#define YAW_KP              0.4
#define YAW_KI              0.016

#define MAX_ALTITUDE_TARGET 100
#define MIN_ALTITUDE_TARGET 0
#define MAX_DUTY_CYCLE      98
#define MIN_DUTY_CYCLE      2

static int32_t yawTarget = 0;
static int32_t altitudeTarget = 0;
static int32_t yawIntegral = 0;
static int32_t altitudeIntegral = 0;
static uint32_t tailDutyCycle;
static uint32_t mainDutyCycle;

/**
 * Sets the target yaw for the PID
 */
void setYawTarget(int32_t target) {
    yawTarget = target;
}

/**
 * Sets the target altitude for the PID
 */
void setAltitudeTarget(int32_t target) {
    altitudeTarget = target;
}

/**
 * Increases the target altitude by a set amount (can be negative). If the
 * target is set above the maximum target, the target will be set to the
 * maximum. If the target is set below the minimum, the target will be set to
 * the minimum.
 */
void increaseAltitudeTarget(int32_t change)
{
    if (altitudeTarget + change > MAX_ALTITUDE_TARGET) {
        altitudeTarget = MAX_ALTITUDE_TARGET;
    } else if (altitudeTarget + change < MIN_ALTITUDE_TARGET) {
        altitudeTarget = MIN_ALTITUDE_TARGET;
    } else {
        altitudeTarget += change;
    }
}

/**
 * Increases the yaw target by a set amount (can be negative).
 */
void increaseYawTarget(int32_t change)
{
    yawTarget += change;
}

/**
 * Returns the current target yaw
 */
int32_t getYawTarget(void)
{
    return yawTarget;
}

/**
 * Returns the current target altitude
 */
int32_t getAltitudeTarget(void)
{
    return altitudeTarget;
}

/**
 * PID control used to control the duty cycle of the main rotor
 * and the tail rotor. Will set the duty cycle of the rotors to what is
 * calculated.
 */
void updatePID()
{
    int32_t altitudeError = altitudeTarget - getAltitudePercentage();
    altitudeIntegral += altitudeError;
    mainDutyCycle = ALTITUDE_KP * altitudeError + ALTITUDE_KI * altitudeIntegral;

    int32_t yawError = yawTarget - getYaw();
    yawIntegral += yawError;
    tailDutyCycle = YAW_KP * yawError + YAW_KI * yawIntegral;

    if (mainDutyCycle > MAX_DUTY_CYCLE) {
        mainDutyCycle = MAX_DUTY_CYCLE;
    } else if (mainDutyCycle < MIN_DUTY_CYCLE) {
        mainDutyCycle = MIN_DUTY_CYCLE;
    }

    if (tailDutyCycle > MAX_DUTY_CYCLE) {
        tailDutyCycle = MAX_DUTY_CYCLE;
    } else if (tailDutyCycle < MIN_DUTY_CYCLE) {
        tailDutyCycle = MIN_DUTY_CYCLE;
    }

    setTailRotorDuty(tailDutyCycle);
    setMainRotorDuty(mainDutyCycle);
}
