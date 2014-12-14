#include <stdio.h>

#include "network.h"

int alwaysCorrupt(int dataFrames, int ackFrames, FrameType frameType) {
	return 1;
}

void main(int argc, char *argv[]) {
	phy_host();
	phy_setSocket(phy_accept());

	//dat_setShouldCorrupt(alwaysCorrupt);

	FILE *file = fopen("output.jpg", "w");

	int endOfPhoto;
	do {
		char buffer[200];
		int received = net_recv(buffer, 200, &endOfPhoto);
		fwrite(buffer, 1, received, file);
	} while (!endOfPhoto);

	//net_send("Hello world from the server! This is a longer response that will not fit inside one frame because it is longer than one hundred and twenty four bytes.", 151, 1);

	phy_close();
	fclose(file);
}
