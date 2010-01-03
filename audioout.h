#define CHUNK_SIZE 4096
#define NUM_CHUNKS 8
#define BUFFSIZE ((NUM_CHUNKS + 1) * CHUNK_SIZE)

/* buffer pointer */
static unsigned char *buffer;

/* may only be modified by SDL's playback thread or while it is stopped */
static volatile int read_pos;
/* may only be modified by outside threads */
static volatile int write_pos;

static int buf_free(void);
static int buf_used(void);
static int write_buffer(unsigned char* data, int len);
static int read_buffer(unsigned char* data, int len);
void buffcallback(void *unused, Uint8 *stream, int len);

typedef struct {
	unsigned int sampleRate;
	unsigned int bufferSize;
	unsigned int outputBufferSize;
} audioInfo;

int sdlinit(void (*callback)(void *, Uint8 *, int), audioInfo *);
