#include "error.h"

/* Author: Ben McMorran
 * Prints the given error, then exits. */
void error_user(const char *msg, const char *detail) {
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(1);
}

/* Author: Ben McMorran
 * Prints the given error with system details, then exits. */
void error_system(const char *msg) {
  perror(msg);
  exit(1);
}

/* Author: Ben McMorran
 * Prints the given error, but does not halt the program. */
void error_log_user(const char *msg, const char *detail) {
  fputs(msg, logfile);
  fputs(": ", logfile);
  fputs(detail, logfile);
  fputs("\n\n", logfile);
}
