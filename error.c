#include "error.h"

FILE* logfile = 0;

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

/* Author: Ben McMorran
 * Logs the given message. */
void log_msg(const char *msg) {
  if (logfile != 0) {
    fputs(msg, logfile);
    fputs("\n", logfile);
  }
}

/* Author: Ben McMorran
 * Logs that the given packet was sent. */
void log_packetSent(int packet) {
  char buffer[50];
  sprintf(buffer, "NET: packet %d sent", packet);
  log_msg(buffer);
}

/* Author: Ben McMorran
 * Logs that an ACK packet was sent. */
void log_ackPacketSent() {
  log_msg("NET: ACK packet sent");
}

/* Author: Ben McMorran
 * Logs that an ACK packet was received. */
void log_ackPacketReceived() {
  log_msg("NET: ACK packet received");
}

/* Author: Ben McMorran
 * Logs that the given frame was sent. */
void log_frameSent(int packet, int frame) {
  char buffer[50];
  sprintf(buffer, "DAT: frame %d of packet %d sent", frame, packet);
  log_msg(buffer);
}

/* Author: Ben McMorran
 * Logs that the given frame was resent. */
void log_frameResent(int packet, int frame) {
  char buffer[50];
  sprintf(buffer, "DAT: frame %d of packet %d resent", frame, packet);
  log_msg(buffer);
}

/* Author: Ben McMorran
 * Logs that the given frame was received. */
void log_frameReceived(int packet, int frame) {
  char buffer[50];
  sprintf(buffer, "DAT: frame %d of packet %d received", frame, packet);
  log_msg(buffer);
}

/* Author: Ben McMorran
 * Logs that a duplicate frame was received. */
void log_duplicateFrameReceived(int packet, int frame) {
  char buffer[70];
  sprintf(buffer, "DAT: duplicate of frame %d of packet %d received", frame, packet);
  log_msg(buffer);
}

/* Author: Ben McMorran
 * Logs that the given ACK frame was sent. */
void log_ackFrameSent(int frame) {
  char buffer[50];
  sprintf(buffer, "DAT: ACK for sequence %d sent", frame);
  log_msg(buffer);
}

/* Author: Ben McMorran
 * Logs that the given ACK frame was successfully received. */
void log_ackFrameReceived(int frame) {
  char buffer[70];
  sprintf(buffer, "DAT: ACK for sequence %d successfully received", frame);
  log_msg(buffer);
}

/* Author: Ben McMorran
 * Logs that a frame was received with errors. */
void log_frameError() {
  log_msg("DAT: frame received in error");
}

/* Author: Ben McMorran
 * Logs that a timeout occured. */
void log_timeout() {
  log_msg("DAT: timer expired");
}
