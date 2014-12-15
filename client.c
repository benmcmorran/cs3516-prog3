#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "error.h"

int main(int argc, char* argv[])
{
	
	// File pointer and strings to concatenate to make 
	FILE *ifp, *ofp;
	char photoFile[255];

	logfile = fopen("clientlog.txt", "w");
	
	
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
				printf("photofile test %s \n " , photoFile);
			
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
 		
	} 
	else 
	{
		printf("Please use the format, ./client servermachine id num_photos");
			exit(1);
	}
	
	fclose(logfile);
    return 0;
} 
