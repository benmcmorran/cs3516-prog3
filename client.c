#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "network.h"
#include "error.h"

// Author: Gordon Gao
// Implements the client of the application layer
int main(int argc, char* argv[])
{
	struct timeval start, end;

	// File pointer and strings to concatenate to make 
	FILE *ifp;
	char photoFile[255];	
	
	// Creates string for file to load
	static int id;
	#define BUFFER_SIZE 200
	int DEBUG = 1;	
	int i;
	
	// Checks for correct input
	if(argc == 4)
	{	
	
		id = atoi(argv[2]);
		int numphotos = atoi(argv[3]);
		char filename[20];
		sprintf(filename, "client_%d.log", id);
		logfile = fopen(filename, "w");
		
		gettimeofday(&start, NULL);

		// Initializes handshake between client and server.
		net_connect(id,numphotos, argv[1]);
		
		for(i = 0;i < numphotos; i++)
		{
			strcpy ( photoFile, "photo");
			// Appends input into one string for file access
			// photoij.jpg
			strcat(photoFile, argv[2]);
			char photoIndex[5];
			sprintf(photoIndex, "%d", i);
			strcat(photoFile, photoIndex);
			strcat(photoFile, ".jpg");

			// Debug to check which file is being opened
			if(DEBUG)
				printf("Sending %s...\n" , photoFile);
			
			ifp = fopen(photoFile, "r");
			
			// Error Check if cannot open file
			if (ifp == NULL) 
			{
			  printf("Can't open input file! \n");
			  exit(1);
			}	 
			
			// Buffer set to 200 bytes
			char buffer[200];
			
			int len;
			do 
			{
				len = fread(buffer,1,200,ifp); 
				net_send(buffer,  len, len != 200);
			} while (len == 200);

		} // end of for loop
		
		phy_close();

		gettimeofday(&end, NULL);

		char message[250];
		unsigned long long int startMicros = start.tv_sec * 1000000 + start.tv_usec;
		unsigned long long int endMicros = end.tv_sec * 1000000 + end.tv_usec;
		sprintf(message, "Total transfer time: %llu seconds and %llu microseconds", (endMicros - startMicros) / 1000000, (endMicros - startMicros) % 1000000);
		log_msg(message);
		sprintf(message, "Frames sent: %d, frame retransmissions: %d, good ACKs: %d, error ACKs: %d", totalFrames, totalFramesResent, totalAcks, totalAckErrors);
		log_msg(message);

 		fclose(logfile);
	} 
	else 
	{
		printf("Please use the format, ./client servermachine id num_photos");
			exit(1);
	}
	
    return 0;
} 
