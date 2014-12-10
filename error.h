#pragma once

#include <stdio.h>
#include <stdlib.h>

// The file to log errors to
FILE* logfile;

// Error function prototypes. See error.c for full documentation.
void error_user(const char *msg, const char *detail);
void error_system(const char *msg);
void error_log_user(const char *msg, const char *detail);
