/*
 * ENCE 361 Group Helicopter Project
 * Helicopter circular buffer code
 *
 * Authors: T.Bingham, I.Bus, S.Booth
 * Original code from P.J.Bones UCECE
 * Last modified: 03.06.2018
 */

#include <stdint.h>
#include "stdlib.h"
#include "circBufT.h"

/**
 * Initialise the circBuf instance. Reset both indices to the start of the
 * buffer.  Dynamically allocate and clear the the memory and return a pointer
 * for the data.  Return NULL if allocation fails.
 */
uint32_t* initCircBuf(circBuf_t* buffer, uint32_t size)
{
    buffer->windex = 0;
    buffer->rindex = 0;
    buffer->size = size;
    buffer->data = (uint32_t*)calloc(size, sizeof(uint32_t));
    return buffer->data;
}

/**
 * Insert entry at the current windex location, advance windex, modulo (buffer
 * size).
 */
void writeCircBuf(circBuf_t* buffer, uint32_t entry)
{
    buffer->data[buffer->windex] = entry;
    buffer->windex++;
    if (buffer->windex >= buffer->size) {
        buffer->windex = 0;
    }
}

/**
 * Return entry at the current rindex location, advance rindex, modulo (buffer
 * size). The function deos not check if reading has advanced ahead of writing.
 */
uint32_t readCircBuf(circBuf_t* buffer)
{
    uint32_t entry;

    entry = buffer->data[buffer->rindex];
    buffer->rindex++;
    if (buffer->rindex >= buffer->size) {
        buffer->rindex = 0;
    }
    return entry;
}

/**
 * Releases the memory allocated to the buffer data, sets pointer to NULL and
 * other fields to 0. The buffer can re-initialised by another call to
 * initCircBuf().
 */
void freeCircBuf(circBuf_t* buffer)
{
    buffer->windex = 0;
    buffer->rindex = 0;
    buffer->size = 0;
    free (buffer->data);
    buffer->data = NULL;
}

