#include "data_link.h"

int neverCorrupt(int dataFrames, int ackFrames, FrameType frameType) {
	return 0;
}

int (*SHOULD_CORRUPT)(int, int, FrameType) = neverCorrupt;
int dataFrameCount = 0;
int ackFrameCount = 0;
uint16_t sequence = 0;

void dat_setShouldCorrupt(int (*corrupt)(int, int, FrameType frameType)) {
	SHOULD_CORRUPT = corrupt;
}

void dat_error(char *data, size_t length, char *error) {
	int i;
	for (i = 0; i < length; i += 2)
		error[0] ^= data[i];
	for (i = 1; i < length; i += 2)
		error[1] ^= data[i];
}

void dat_send(char *data, size_t length) {
	char error[2] = { 0, 0 };
	dat_error(data, length, error);
	phy_send(data, length, error, SHOULD_CORRUPT(dataFrameCount, ackFrameCount, DATA));
}

size_t dat_recv(char *data, size_t length) {
	char buffer[200];
	size_t frameLength = phy_recv(buffer, 200);

	char *frameError = (char *) buffer + frameLength - 2;
	char expectedError[] = { 0, 0 };
	dat_error(buffer, frameLength - 2, frameError);

	if (frameError[0] != expectedError[0] || frameError[1] != expectedError[1])
		error_user("dat_recv()", "error detection field invalid");

	memcpy(data, buffer, frameLength - 2);

	return frameLength - 2;
}
