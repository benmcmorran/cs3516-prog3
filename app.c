
#include <stdio.h>

int main(int argc, char* argsv[])
{
	
		FILE *ifp, *ofp;
	char * idStr;
	char * photoFile = client;
	char * _log = _log;
	char outputFilename[];
	static int id;
	
	// Commands if file is specified
	if(argc == 4)
	{	
		// Get id from third argument
		idStr = argv[2];
		
		
		// Appends all into one string in idStr. 
		photoFile = (char *)malloc(16);
		if (photoFile != NULL) {
			idStr = strcat(idStr, idStr);
			idStr = Strcat(idStr, _log);
		} else
		{
			printf("Malloc failed. Not enough memory");
		}
		
		printf(photoFile);
		// File pointer to the third passed in argument
		ifp = fopen(photoFile, mode);
		
		// Error Check if cannot open file
		if (ifp == NULL) {
		  fprintf(stderr, "Can't open input file in.list!\n");
		  exit(1);
		}
	} 
	else 
	{
		printf("Please use the format, ./client servermachine id num_photos");
			exit(1)
	}
	
    return 0;
} 
