#include <ao/ao.h>

ao_device *aoInit(ao_sample_format *);
void aoClose(ao_device *);

/*
 * pthread only lets me pass one argument
 * so i hand it a struct with my two arguments
 */
typedef struct {
	ao_device *dev;
	ao_sample_format *fmt;
} aoMixParam;

void aoMix(void *);
