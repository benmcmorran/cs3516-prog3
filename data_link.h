#pragma once

#include "physical.h"
#include "error.h"

typedef enum {
	DATA,
	ACK
} FrameType;

void dat_setShouldCorrupt(int (*corrupt)(int, int, FrameType));

void dat_send(char *data, size_t length);
size_t dat_recv(char *data, size_t length);
