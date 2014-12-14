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


// Breaks apart data buffer.
// Is keeping track of the field types in the packet. 
// then strips it to just the char in the payload.
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

// Breaks apart data buffer.
void net_recvACKPacket() {
	char buffer;
	dat_recv(&buffer, 1);
	
	
	if(buffer != PT_ACK)
	{
		printf("Is not ACK packet");
		exit(1);
	}
	
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
	
	// Condenses into one char buffer
	makePacket(pinfo, len, end, data,payload);
	// Length of packet payload and the three 
	totalLen = length + 3;
	
	
	dat_send(payload, totalLen);
	// Wait for ACK
	net_recvACKPacket();
}


// Only handles data packets
// Returns how much data was read
// length is not length of data in the buffer.
// Represents the size of it.
// Only called by server. Sends an ACK to client network layer.
int net_recv(char * data, size_t length, int *endOfPhoto) 
{	
	// buffer for ack to send. Only 1 byte.
	char ack = PT_ACK;
	int pinfo;
	uint8_t  plen, pEOP;
	
	net_recvDataPacket( &pinfo, &plen, &pEOP, data);
	*endOfPhoto = pEOP;
	// Sends ACK after receiving packet from data link layer.
	dat_send(&ack, 1);
	return plen;
}



void net_connect( uint16_t id, char * server) {
	
}
