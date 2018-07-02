/*
 * ENCE 361 Group Helicopter Project
 * Helicopter altitude code
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
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "inc/tm4c123gh6pm.h"
#include "circBufT.h"
#include "altitude.h"

#define ALTITUDE_PERIPHERAL          SYSCTL_PERIPH_ADC0
#define ALTITUDE_BASE                ADC0_BASE
#define ALTITUDE_INT                 INT_GPIOB
#define ADC_SEQUENCE_NUMBER          3

#define ALTITUDE_BUFFER_SIZE         12
#define ALTITUDE_SAMPLE_RATE_HZ      120
#define HEIGHT_VOLTAGE_DIFFERENCE    0.8
#define ALTITUDE_REGRESSION_CONSTANT 1262

static circBuf_t g_inBuffer;
static int16_t groundValue;
static int16_t topValue;
static volatile int16_t heightValue;

/**
 * Triggers an ADC sample sequence. Once this sample sequence is complete, the
 * ADC module will set an interrupt flag to call ADCIntHandler()
 */
static void SysTickIntHandler(void)
{
    ADCProcessorTrigger(ADC0_BASE, ADC_SEQUENCE_NUMBER);
}

/**
 * Initialise SysTick to trigger an interrupt at ALTITUDE_SAMPLE_RATE_HZ
 * frequency
 */
static void initSysTick(void)
{
    SysTickPeriodSet(SysCtlClockGet() / ALTITUDE_SAMPLE_RATE_HZ);
    SysTickIntRegister(SysTickIntHandler);
    SysTickIntEnable();
    SysTickEnable();
}

/**
 * Writes the current ADC value to the circular buffer
 */
static void ADCIntHandler(void)
{
    uint32_t ulValue;
    ADCSequenceDataGet(ALTITUDE_BASE, ADC_SEQUENCE_NUMBER, &ulValue);
    ADCIntClear(ALTITUDE_BASE, ADC_SEQUENCE_NUMBER);
    writeCircBuf(&g_inBuffer, ulValue);
}

static void initADC(void)
{
    SysCtlPeripheralEnable(ALTITUDE_PERIPHERAL);
    ADCSequenceConfigure(ALTITUDE_BASE, ADC_SEQUENCE_NUMBER, ADC_TRIGGER_PROCESSOR, 0);
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(ALTITUDE_BASE, ADC_SEQUENCE_NUMBER, 0, ADC_CTL_CH9 | ADC_CTL_IE |
            ADC_CTL_END);
    ADCSequenceEnable(ALTITUDE_BASE, ADC_SEQUENCE_NUMBER);
    ADCIntRegister(ALTITUDE_BASE, ADC_SEQUENCE_NUMBER, ADCIntHandler);
    ADCIntEnable(ALTITUDE_BASE, ADC_SEQUENCE_NUMBER);
}

/**
 * Returns the average value in the circular buffer
 */
static int32_t calcBufferAverage(void)
{
    uint16_t i;
    int32_t sum = 0;
    for (i = 0; i < ALTITUDE_BUFFER_SIZE; i++) {
        sum = sum + readCircBuf(&g_inBuffer);
    }
    return ((2 * sum + ALTITUDE_BUFFER_SIZE) / 2 / ALTITUDE_BUFFER_SIZE);
}

/**
 * Initialise everything needed for the altitude calculation
 */
void initAltitude(void)
{
    initADC();
    initSysTick();
    initCircBuf(&g_inBuffer, ALTITUDE_BUFFER_SIZE);
    /*
     * Delay to populate buffer
     */
    SysCtlDelay(SysCtlClockGet() / 6);
    groundValue = calcBufferAverage();
    topValue = groundValue - (ALTITUDE_REGRESSION_CONSTANT * HEIGHT_VOLTAGE_DIFFERENCE);
}

/**
 * Updates the altitude calculation
 */
void updateAltitude(void)
{
    heightValue = calcBufferAverage();
}

/**
 * Recalibrates the reference ground value
 */
void recalibrateAltitude(void)
{
    groundValue = heightValue;
    topValue = groundValue - (ALTITUDE_REGRESSION_CONSTANT * HEIGHT_VOLTAGE_DIFFERENCE);
}

/**
 * Return the percentage height (0% == ground, 100% == top)
 */
int32_t getAltitudePercentage(void)
{
    int32_t percentage = ((float)(heightValue - groundValue) / (float)(topValue - groundValue)) * 100;
    return percentage;
}

/**
 * Return the ADC calculated height value
 */
int16_t getHeightValue(void)
{
    return heightValue;
}

/**
 * Return the ADC calculated top value
 */
int16_t getTopValue(void)
{
    return topValue;
}

/**
 * Return the ADC calculated ground value
 */
int16_t getGroundValue(void)
{
    return groundValue;
}
