
typedef enum {
	DATA,
	ACK
} FrameType;

extern int (*shouldCorrupt)(int, int, enum FrameType);
void dat_setShouldCorrupt(int (*corrupt)(int, int, enum FrameType));

void dat_send(char *data, size_t length);
size_t dat_recv(char *data, size_t length);
