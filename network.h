#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_link.h"

void net_send(char * data, size_t length, int end);
int net_recv(char * data, size_t length);
void net_connect( uint16_t id, char * server);
