#pragma once

/* Defines the data link layer interface. */

#include "physical.h"
#include "error.h"

extern int totalFrames;
extern int totalFramesResent;
extern int totalAcks;
extern int totalAckErrors;

/* Define possible frame types. */
typedef enum {
	FT_DATA = 0,
	FT_ACK = 1
} FrameType;

/* Data link function prototypes. See data_link.c for full documentation. */
void dat_setShouldCorrupt(int (*corrupt)(int, int, FrameType));
void dat_send(char *data, size_t length);
size_t dat_recv(char *data, size_t length);
