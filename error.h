#pragma once

#include <stdio.h>
#include <stdlib.h>

// The file to log errors to
extern FILE* logfile;

// Error function prototypes. See error.c for full documentation.
void error_user(const char *msg, const char *detail);
void error_system(const char *msg);
void error_log_user(const char *msg, const char *detail);

// Logging fuction prototypes. See error.c for full documentation.
void log_msg(const char *msg);
void log_packetSent(int packet);
void log_ackPacketSent();
void log_ackPacketReceived();
void log_frameSent(int packet, int frame);
void log_frameResent(int packet, int frame);
void log_frameReceived(int packet, int frame);
void log_duplicateFrameReceived(int packet, int frame);
void log_ackFrameSent(int frame);
void log_ackFrameReceived(int frame);
void log_frameError();
void log_timeout();
