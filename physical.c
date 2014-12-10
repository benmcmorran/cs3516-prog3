#include "physical.h"

int SOCKET = -1;

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

int phy_accept() {
	struct sockaddr_in addr;
	socklen_t addrLen = sizeof(addr);
	int sock = accept(SOCKET, (struct sockaddr *) &addr, &addrLen);
	if (sock < 0) error_system("accept() failed");
	return sock;
}

void phy_close() {
	if (close(SOCKET) < 0) error_system("close() failed");
}

void phy_setSocket(int sock) {
	SOCKET = sock;
}

void phy_sendBuffer(char *data, size_t length) {
	ssize_t sent = send(SOCKET, data, length, 0);
	if (sent < 0) error_system("send() failed");
	else if (sent != length) error_user("send()", "sent unexpected number of bytes");
}

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

size_t phy_recvPartial(char *data, size_t length) {
	ssize_t received = recv(SOCKET, data, length, 0);
	if (received < 0) error_system("recv() failed");
	else if (received == 0) error_user("recv()", "connection closed prematurely");
	return received;
}

void phy_recvBuffer(char* data, size_t length) {
	size_t received = 0;
	while (received < length)
		received += phy_recvPartial((char *) data + received, length - received);
}

size_t phy_recv(char *data, size_t length) {
	uint8_t frameLength;
	phy_recvBuffer(&frameLength, 1);

	if (frameLength > length) error_user("recv()", "frame is larger than buffer");

	phy_recvBuffer(data, frameLength);
	return frameLength;
}
