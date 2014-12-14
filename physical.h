#pragma once

/* Defines the physical layer interface. */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "error.h"

/* Establish the well known port and backlog size. */
#define PORT 32789
#define BACKLOG 5

/* Connection and server function prototypes. See physical.c for full documentation. */
void phy_connect(char *server);
void phy_host();
int phy_accept();
void phy_close();
void phy_setSocket(int sock);

/* Send and receive function prototpyes. See physical.c for full documentation. */
void phy_send(char *data, size_t length, char *error, int corrupt);
size_t phy_recv(char *data, size_t length);
