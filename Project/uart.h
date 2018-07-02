/*
 * ENCE 361 Group Helicopter Project
 * Helicopter uart header file
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Original code from P.J.Bones UCECE
 * Last modified: 03.06.2018
 */

#ifndef UART_H_
#define UART_H_

/**
 * Initialise the UART module
 */
void initUART(void);

/**
 * Sends a string of characters through the UART (blocking)
 */
void sendUART(char *pucBuffer);

#endif /* UART_H_ */
