/*
 * ENCE 361 Group Helicopter Project
 * Helicopter yaw header file
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Last modified: 03.06.2018
 */

#ifndef YAW_H_
#define YAW_H_

#include <stdbool.h>

/**
 * Initialise the yaw module
 */
void initYaw(void);

/**
 * Updates the yaw
 */
void updateYaw(void);

/**
 * Returns the yaw in degrees
 */
int32_t getYaw(void);

/**
 * Returns whether the reference position has been initialised
 */
bool checkReferenceInitialised(void);

#endif /* YAW_H_ */
