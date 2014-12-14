#include "physical.h"

/* Global socket identifier used for physical layer communication. */
int SOCKET = -1;

/* Default timeout value for receiving data */
struct timeval TIMEOUT = {
	.tv_sec = 0,
	.tv_usec = 2000
};

/* Author: Ben McMorran
 * Establishes a connection with the server at the given hostname. */
void phy_connect(char *server) {
	SOCKET = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET < 0) error_system("socket() failed");

	struct addrinfo hints = {
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM
	}, *addrInfo;

	int rv = getaddrinfo(server, 0, &hints, &addrInfo);
	if (rv != 0) error_user("getaddrinfo() failed", gai_strerror(rv));
	struct in_addr addr = ((struct sockaddr_in *) addrInfo->ai_addr)->sin_addr;
	freeaddrinfo(addrInfo);
	
	struct sockaddr_in connectAddr = {
		.sin_family = AF_INET,
		.sin_addr = addr,
		.sin_port = htons(PORT)
	};
	if (connect(SOCKET, (struct sockaddr *) &connectAddr, sizeof(connectAddr)) < 0)
		error_system("connect() failed");
}

/* Author: Ben McMorran
 * Configures this device to act as a server by creating a socket, binding to
 * the well known port, and setting the socket to listen. */
void phy_host() {
	SOCKET = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET < 0) error_system("socket() failed");

	struct sockaddr_in bindAddr = {
		.sin_family = AF_INET,
		.sin_port = htons(PORT)
	};
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(SOCKET, (struct sockaddr *) &bindAddr, sizeof(bindAddr)) < 0)
		error_system("bind() failed");

	if (listen(SOCKET, BACKLOG) < 0) error_system("listen() failed");
}

/* Author: Ben McMorran
 * Blocks until an incoming connection arrives, returning the client socket. */
int phy_accept() {
	struct sockaddr_in addr;
	socklen_t addrLen = sizeof(addr);
	int sock = accept(SOCKET, (struct sockaddr *) &addr, &addrLen);
	if (sock < 0) error_system("accept() failed");
	return sock;
}

/* Author: Ben McMorran
 * Closes the physical connection. */
void phy_close() {
	if (close(SOCKET) < 0) error_system("close() failed");
}

/* Author: Ben McMorran
 * Sets the global socket to sock. Used in conjunction with phy_accept. */
void phy_setSocket(int sock) {
	SOCKET = sock;
}

/* Author: Ben McMorran
 * Prints the hexadecimal and ASCII values in buf. Used for debugging. */
void phy_printBuffer(char *buf, size_t length) {
	int i, j;
	for (i = 0; i < length;) {
		for (j = i; j < i + 8 && j < length; j++) {
			printf("%02x ", (uint8_t)buf[j]);
		}
		for (j = i; j < i + 8 && j < length; j++) {
			printf("%c  ", buf[j]);
		}
		printf("\n");
		i = j;
	}
}

/* Author: Ben McMorran
 * Sends the given buffer over the TCP connection. */
void phy_sendBuffer(char *data, size_t length) {
	ssize_t sent = send(SOCKET, data, length, 0);
	if (sent < 0) error_system("send() failed");
	else if (sent != length) error_user("send()", "sent unexpected number of bytes");

	/* 
	// Debug code
	printf("phy send\n");
	phy_printBuffer(data, length);
	printf("\n");
	*/
}

/* Author: Ben McMorran
 * Sends the given buffer and error detection bytes over the physical layer,
 * optionally flipping a random bit in error if corrupt is true. */
void phy_send(char *data, size_t length, char *error, int corrupt) {
	if (length + 2 > 255) error_user("send()", "length is outside range");

	if (corrupt) {
		int bit = rand() % 16;
		*(uint16_t *)error = *(uint16_t *)error ^ (0x1 << bit);
	}

	uint8_t frameLength = length + 2;
	phy_sendBuffer(&frameLength, 1);
	phy_sendBuffer(data, length);
	phy_sendBuffer(error, 2);
}

/* Author: Ben McMorran
 * Receives up to length bytes from the physical layer into data, returning the
 * number of bytes actually received. */
ssize_t phy_recvPartial(char *data, size_t length) {
	fd_set fdSet;
	int readyNo;
	
	FD_ZERO(&fdSet);
	FD_SET(SOCKET, &fdSet);
	readyNo = select(SOCKET + 1, &fdSet, 0, 0, &TIMEOUT);
	
	if (readyNo < 0) error_system("select() failed");
	else if (readyNo == 0) return -1;
	
	ssize_t received = recv(SOCKET, data, length, 0);
	if (received < 0) error_system("recv() failed");
	else if (received == 0) error_user("recv()", "connection closed prematurely");
	
	/*
	// Debug info 
	printf("phy recv\n");
	phy_printBuffer(data, received);
	printf("\n");
	*/
	
	return received;
}

/* Author: Ben McMorran
 * Receives from the physical layer into data until the buffer is filled. */
ssize_t phy_recvBuffer(char* data, size_t length) {
	size_t received = 0;
	while (received < length) {
		ssize_t count = phy_recvPartial((char *) data + received, length - received);
		if (count < 0) return -1;
		received += count;
	}
	return received;
}

/* Author: Ben McMorran
 * Receives one frame from the physical layer and places it in data, returning
 * the length of the frame in bytes. */
ssize_t phy_recv(char *data, size_t length) {
	uint8_t frameLength;
	if (phy_recvBuffer(&frameLength, 1) < 0) return -1;

	if (frameLength > length) error_user("recv()", "frame is larger than buffer");
	
	if (phy_recvBuffer(data, frameLength) < 0) return -1;
	return frameLength;
}
