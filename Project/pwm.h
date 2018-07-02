/*
 * ENCE 361 Group Helicopter Project
 * Helicopter pwm header file
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Original code from P.J.Bones UCECE
 * Last modified: 03.06.2018
 */

#ifndef PWM_H_
#define PWM_H_

/**
 * Initialise the two PWM generators that drive the main rotor and the tail
 * rotor.
 */
void initPWM(void);

/**
 * Sets the duty cycle for the tail rotor PWM generator
 */
void setTailRotorDuty(uint32_t duty);

/**
 * Sets the duty cycle for the main rotor PWM generator
 */
void setMainRotorDuty(uint32_t duty);

/**
 * Returns the current duty cycle of the tail rotor PWM generator
 */
uint32_t getTailRotorDuty(void);

/**
 * Returns the current duty cycle of the main rotor PWM generator
 */
uint32_t getMainRotorDuty(void);

#endif /* PWM_H_ */
