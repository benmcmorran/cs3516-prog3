#include "data_link.h"

int neverCorrupt(int dataFrames, int ackFrames, enum FrameType frameType) {
	return 0;
}

int (*shouldCorrupt)(int, int, enum FrameType) = neverCorrupt;
int dataFrameCount = 0;
int ackFrameCount = 0;
unit_16 sequence = 0;

void dat_setShouldCorrupt(int (*corrupt)(int, int, enum FrameType frameType)) {
	shouldCorrupt = corrupt;
}

void dat_send(char *data, size_t length) {
	// Stub for now
	char error[2];
	phy_send(data, length, error, shouldCorrupt(dataFrameCount, ackFrameCount, DATA));
}

size_t dat_recv(char *data, size_t length) {
	// Stub for now
	char buffer[100];
	return phy_recv(data, 100);
}
