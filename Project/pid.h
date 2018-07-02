/*
 * ENCE 361 Group Helicopter Project
 * Helicopter pid headre file
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Last modified: 03.06.2018
 */

#ifndef PID_H_
#define PID_H_

/**
 * Returns the current target yaw
 */
int32_t getYawTarget(void);

/**
 * Returns the current target altitude
 */
int32_t getAltitudeTarget(void);

/**
 * Increases the target altitude by a set amount (can be negative). If the
 * target is set above the maximum target, the target will be set to the
 * maximum. If the target is set below the minimum, the target will be set to
 * the minimum.
 */
void increaseAltitudeTarget(int32_t change);

/**
 * Increases the yaw target by a set amount (can be negative).
 */
void increaseYawTarget(int32_t change);

 /**
 * Sets the target altitude for the PID
 */
void setAltitudeTarget(int32_t target);

/**
 * Sets the target yaw for the PID
 */
void setYawTarget(int32_t target);

/**
 * PID control used to control the duty cycle of the main rotor
 * and the tail rotor. Will set the duty cycle of the rotors to what is
 * calculated.
 */
void updatePID(void);

#endif /* PID_H_ */
