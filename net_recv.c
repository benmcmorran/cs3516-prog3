// Only handles data packets
// Returns how much data was read


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// length is not length of data in the buffer.
// Represents the size of it.
int net_recv(char * data, size_t length) 
{	

	int pinfo;
	uint8_t  plen, pEOP;
	net_recvDataPacket( &pinfo, &plen, &pEOP, data);
	return plen;	
	
}


// Breaks apart data buffer.
void net_recvDataPacket( int *pinfo, uint8_t *len, uint8_t *EOP, char * data) {
	char buffer[250];
	dat_recv(buffer, 250);
	*pinfo = buffer[0];
	*len = buffer[1];
	*EOP = buffer[2];

	
	if( *pinfo != 1)
	{
		printf("Is not data packet");
		exit(1);
	}
	
	memcpy(data,buffer+3, len);
	
}

