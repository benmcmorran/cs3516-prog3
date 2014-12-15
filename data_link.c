#include "data_link.h"

/* Author: Ben McMorran
 * Conveince function to never corrupt outgoing frames. */
int neverCorrupt(int dataFrames, int ackFrames, FrameType frameType) {
	return 0;
}

/* Global variable to decide corruption, track frames, and track sequence. */
int (*SHOULD_CORRUPT)(int, int, FrameType) = neverCorrupt;
int dataFrameCount = 0;
int ackFrameCount = 0;
uint16_t sequence = 0;

/* Author: Ben McMorran
 * Sets the global corruption decision function to allow custom behavior. */
void dat_setShouldCorrupt(int (*corrupt)(int, int, FrameType frameType)) {
	SHOULD_CORRUPT = corrupt;
}

/* Author: Ben McMorran
 * Computes the XOR fold of data, storing the result in error. */
void dat_error(char *data, size_t length, char *error) {
	int i;
	for (i = 0; i < length; i += 2)
		error[0] ^= data[i];
	for (i = 1; i < length; i += 2)
		error[1] ^= data[i];
}

/* Author: Ben McMorran
 * Verifies that the given frame has the correct error detection bits. */
int dat_verifyFrame(char *data, size_t length, char *expectedError) {
	char frameError[] = { 0, 0 };
	dat_error(data, length, frameError);
	return frameError[0] == expectedError[0] && frameError[1] == expectedError[1];
}

/* Author: Ben McMorran
 * Receives one valid frame from the physical layer, returning its length. */
ssize_t dat_recvRawFrame(char *data, size_t length) {
	ssize_t frameLength;
	do {
		frameLength = phy_recv(data, 200);
		if (frameLength < 0) return -1;
	} while (!dat_verifyFrame(data, frameLength - 2, data + frameLength - 2));
	return frameLength - 2;
}

/* Author: Ben McMorran
 * Receives one valid data frame, filling in the appropritate arguments. */
int dat_recvDataFrame(uint16_t *sequenceNumber, uint8_t *endOfPacket, uint8_t *length, char *payload) {
	char buffer[200];
	if (dat_recvRawFrame(buffer, 200) < 0) return -1;

	if ((uint8_t)buffer[0] != FT_DATA)
		error_user("dat_recvDataFrame()", "received non data frame");

	*sequenceNumber = *(uint16_t *)(buffer + 1);
	*endOfPacket = (uint8_t)buffer[3];
	*length = (uint8_t)buffer[4];
	memcpy(payload, buffer + 5, *length);
	return 0;
}

/* Author: Ben McMorran
 * Receives one valid ack frame, filling in sequenceNumber. */
int dat_recvAckFrame(uint16_t *sequenceNumber) {
	char buffer[5];
	if (dat_recvRawFrame(buffer, 5) < 0) return -1;

	if ((uint8_t)buffer[0] != FT_ACK)
		error_user("dat_recvAckFrame()", "received non ack frame");

	*sequenceNumber = *(uint16_t *)(buffer + 1);
	return 0;
}

/* Author: Ben McMorran
 * Computes the error detection bytes of a raw frame and sends it. */
void dat_sendRawFrame(char *data, size_t length) {
	char error[2] = { 0, 0 };
	dat_error(data, length, error);
	phy_send(data, length, error, SHOULD_CORRUPT(dataFrameCount, ackFrameCount, FT_DATA));
}

/* Author: Ben McMorran
 * Sends one data frame with the given parameters. */
void dat_sendDataFrame(uint8_t endOfPacket, char *data, size_t length) {
	char buffer[200];
	buffer[0] = FT_DATA;
	*(uint16_t *)(buffer + 1) = sequence;
	buffer[3] = endOfPacket;
	buffer[4] = length;
	memcpy(buffer + 5, data, length);
	dat_sendRawFrame(buffer, 5 + length);
	dataFrameCount++;
}

/* Author: Ben McMorran
 * Sends one ack frame with the appropriate sequence number. */
void dat_sendAckFrame(uint16_t sequenceNumber) {
	char buffer[3];
	buffer[0] = FT_ACK;
	*(uint16_t *)(buffer + 1) = sequenceNumber;
	dat_sendRawFrame(buffer, 3);
	ackFrameCount++;
}

/* Author: Ben McMorran
 * Sends data across the data link layer, breaking into frames and waiting for
 * acks as needed. */
void dat_send(char *data, size_t length) {
	int i;
	for (i = 0; i < length; i += 124) {
		int end = i + 124 >= length;
		int payloadLength = end ? length - i : 124;
		
		int frameAcked = 0;
		while (!frameAcked) {
			dat_sendDataFrame(end, data + i, payloadLength);

			// Multiple old ACKs may have accumulated. Loop until we timeout or
			// reach an ACK with the correct sequence number.
			uint16_t sequenceNumber;
			int error;
			do {
				int error = dat_recvAckFrame(&sequenceNumber);
				frameAcked = error >= 0 && sequenceNumber == sequence;
			} while (error >= 0 && sequenceNumber != sequence);
		}

		sequence++;
	}
}

/* Author: Ben McMorran
 * Receives one packet from the data link layer, placing the result in data. */
size_t dat_recv(char *data, size_t length) {
	uint8_t end;
	size_t position = 0;
	do {
		uint16_t sequenceNumber;
		uint8_t payloadLength;
		int error = dat_recvDataFrame(&sequenceNumber, &end, &payloadLength, data + position);
		
		// ACK if no error
		if (error >= 0) {
			dat_sendAckFrame(sequenceNumber);
			if (sequenceNumber == sequence) {
				position += payloadLength;
				sequence++;
			}
		}
	} while (!end);

	return position;
}
