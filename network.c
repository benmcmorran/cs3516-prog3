#include "network.h"

typedef enum{
	PT_ACK = 0,
	PT_DATA = 1,
	PT_INIT = 2
} p_type;

// Appends all the bytes together to form one char*
void makePacket( int pinfo,int  len,int EOP,char * data, char * ret)
{
	ret[0] = pinfo;
	ret[1] = len;
	ret[2] = EOP;
	memcpy(&ret[3],data,len);
}

void net_send(char * data, size_t length, int end) 
{
	// paylod will call createPacket and be side to packet created from length, end and data.
	char payload[250];
	int len = length;
	// Return char * for makePacket
	int totalLen;
	// Packet type
	p_type pinfo = PT_DATA;
	
	makePacket(pinfo, len, end, data,payload);
	totalLen = length + 3;
	
	dat_send(payload, totalLen);
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
	
	memcpy(data,buffer+3, *len);
}

// Only handles data packets
// Returns how much data was read
// length is not length of data in the buffer.
// Represents the size of it.
int net_recv(char * data, size_t length) 
{	
	int pinfo;
	uint8_t  plen, pEOP;
	net_recvDataPacket( &pinfo, &plen, &pEOP, data);
	return plen;
}

void net_connect( uint16_t id, char * server) {
	
}
