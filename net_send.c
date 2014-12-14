#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum{
	ACK = 0,
	DATA = 1,
	INIT = 2
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
	p_type pinfo = DATA;
	
	makePacket(pinfo, len, end, data,payload);
	totalLen = length + 3;
	
	dat_send(payload, totalLen);
	
	
	
}

