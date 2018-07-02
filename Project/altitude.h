/*
 * ENCE 361 Group Helicopter Project
 * Helicopter altitude header file
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Original code from P.J.Bones UCECE
 * Last modified: 03.06.2018
 */

#ifndef ALTITUDE_H_
#define ALTITUDE_H_

/**
 * Initialise the altitude module
 */
void initAltitude(void);

/**
 * Updates the altitude calculation
 */
void updateAltitude(void);

/**
 * Recalibrates the reference ground value
 */
void recalibrateAltitude(void);

/**
 * Return the percentage height (0% == ground, 100% == top)
 */
int32_t getAltitudePercentage(void);

/**
 * Return the ADC calculated height value
 */
int16_t getHeightValue(void);

/**
 * Return the ADC calculated top value
 */
int16_t getTopValue(void);

/**
 * Return the ADC calculated ground value
 */
int16_t getGroundValue(void);

#endif /* ALTITUDE_H_ */
