/*
 * ENCE 361 Group Helicopter Project
 * Helicopter display header file
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Last modified: 03.06.2018
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

/**
 * Initialise the OLED display
 */
void initDisplay(void);

/**
 * Updates the OLED display, showing the altitude percentage, yaw, main rotor
 * duty cycle, and the tail rotor duty cycle.
 */
void updateDisplay(void);


#endif /* DISPLAY_H_ */
