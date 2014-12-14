#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"

int main(int argc, char* argv[])
{
	
	// File pointer and strings to concatenate to make 
	FILE *ifp, *ofp;
	char photoFile[255];
	strcpy ( photoFile, "client");
	
	// Creates string for file to load
	static int id;
	#define BUFFER_SIZE 200
	int DEBUG = 1;	
	
	// Checks for correct input
	if(argc == 4)
	{	
		id = atoi(argv[2]);

		// Appends input into one string for file access
		strcat(photoFile, argv[2]);
		strcat(photoFile, "_log");

		// Debug to check which file is being opened
		if(DEBUG)
			printf("photofile test %s \n " , photoFile);
		
		ifp = fopen("test_images/cat.jpg", "r");
		
		// Error Check if cannot open file
		if (ifp == NULL) 
		{
		  printf("Can't open input file! \n");
		  exit(1);
		}	 
		
		phy_connect(argv[1]);

		// Buffer set to 200 bytes
		char buffer[200];
		
		int len;
		do 
		{
			len = fread(buffer,1,200,ifp); 
			net_send(buffer,  len, len != 200);
		} while (len == 200);

		phy_close();
 		
	} 
	else 
	{
		printf("Please use the format, ./client servermachine id num_photos");
			exit(1);
	}
	
    return 0;
} 
