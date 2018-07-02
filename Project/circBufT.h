/*
 * ENCE 361 Group Helicopter Project
 * Helicopter circular buffer header file
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Original code from P.J.Bones UCECE
 * Last modified: 03.06.2018
 */

#ifndef CIRCBUFT_H_
#define CIRCBUFT_H_

#include <stdint.h>

typedef struct {
    uint32_t size;   // Number of entries in buffer
    uint32_t windex; // index for writing, mod(size)
    uint32_t rindex; // index for reading, mod(size)
    uint32_t* data;  // pointer to the data
} circBuf_t;

/**
 * Initialise the circBuf instance. Reset both indices to the start of the
 * buffer.  Dynamically allocate and clear the the memory and return a pointer
 * for the data.  Return NULL if allocation fails.
 */
uint32_t* initCircBuf(circBuf_t* buffer, uint32_t size);

/**
 * Insert entry at the current windex location, advance windex, modulo (buffer
 * size).
 */
void writeCircBuf(circBuf_t* buffer, uint32_t entry);

/**
 * Return entry at the current rindex location, advance rindex, modulo (buffer
 * size). The function deos not check if reading has advanced ahead of writing.
 */
uint32_t readCircBuf(circBuf_t* buffer);

/**
 * Releases the memory allocated to the buffer data, sets pointer to NULL and
 * other fields to 0. The buffer can re initialised by another call to
 * initCircBuf().
 */
void freeCircBuf(circBuf_t* buffer);

#endif /*CIRCBUFT_H_*/
