#include <stdio.h>

#include "network.h"

int ServerCorrupt(int dataFrames, int ackFrames, FrameType frameType) {
	return ackFrames % 13 == 0 && frameType == FT_ACK;
}

void main(int argc, char *argv[]) {
	// phy_host readies server to receive connections
	phy_host();
	// dat_setShouldCorrupt(ServerCorrupt);
	while(1)
	{	
		// Gets new socket
		int clientsock = phy_accept();
		// Forks and sets to new Socket
		if( fork() == 0)
		{
			phy_setSocket(clientsock);
			break;
		}
		
	}
	
	int id,numphotos,i;
	net_handshake(&id, &numphotos);
	char photoFile[100];
	
	
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

