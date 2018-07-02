/*
 * ENCE 361 Group Helicopter Project
 * Helicopter display code
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Last modified: 03.06.2018
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "buttons.h"
#include "stdlib.h"
#include "altitude.h"
#include "yaw.h"
#include "pid.h"
#include "pwm.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "display.h"

/**
 * Initialise the OLED display
 */
void initDisplay(void)
{
    OLEDInitialise();
}

/**
 * Updates the OLED display, showing the altitude percentage, yaw, main rotor
 * duty cycle, and the tail rotor duty cycle.
 */
void updateDisplay(void)
{   
    int32_t altitude = getAltitudePercentage();
    int32_t yaw = getYaw();
    uint32_t mainRotorDuty = getMainRotorDuty();
    uint32_t tailRotorDuty = getTailRotorDuty();
    char string[17]; // 16 characters across the display

    usnprintf(string, sizeof(string), "Altitude =  %4d", altitude);
    OLEDStringDraw(string, 0, 0);
    usnprintf(string, sizeof(string), "Aim =       %4d", yaw);
    OLEDStringDraw(string, 0, 1);
    usnprintf(string, sizeof(string), "Main Duty = %4d", mainRotorDuty);
    OLEDStringDraw(string, 0, 2);
    usnprintf(string, sizeof(string), "Tail Duty = %4d", tailRotorDuty);
    OLEDStringDraw(string, 0, 3);
}

