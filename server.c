#include <stdio.h>

#include "network.h"

int alwaysCorrupt(int dataFrames, int ackFrames, FrameType frameType) {
	return 1;
}

void main(int argc, char *argv[]) {
	// phy_host readies server to receive connections
	phy_host();
	// Creates socket for tcp connection (physical layer)
	phy_setSocket(phy_accept());
	int id,numphotos,i;
	net_handshake(&id, &numphotos);
	char photoFile[100];
	//dat_setShouldCorrupt(alwaysCorrupt);
	
	
	for(i = 0;i < numphotos; i++)
	{
		
		
		strcpy ( photoFile, "photonew");
		// Appends input into one string for file access
		// photoij.jpg
		char idIndex[5];
		sprintf(idIndex,"%d", id);
		strcat(photoFile, idIndex);
		char photoIndex[5];
		sprintf(photoIndex, "%d", i);
		strcat(photoFile, photoIndex);
		strcat(photoFile, ".jpg");
		
		FILE *file = fopen(photoFile, "w");

		int endOfPhoto;
		do {
			char buffer[200];
			int received = net_recv(buffer, 200, &endOfPhoto);
			fwrite(buffer, 1, received, file);
		} while (!endOfPhoto);
		
		// Close each file after loop is complete.
		fclose(file);
	}
	
	//net_send("Hello world from the server! This is a longer response that will not fit inside one frame because it is longer than one hundred and twenty four bytes.", 151, 1);

	phy_close();
	
}

