/*
 * Now using libao - http://www.xiph.org/ao/doc/
 */
#include <stdio.h>
#include <ao/ao.h>
#include <pthread.h>

#include "audioout.h"
#include "voices.h"

ao_device *aoInit(ao_sample_format *fmt) {
	ao_device *dev;
	int def_driver;

	/* init */
	ao_initialize();
	def_driver = ao_default_driver_id();
	/* open device */
	dev = ao_open_live(def_driver, fmt, NULL);
	if(dev == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return NULL;
	}

	return dev;
}

void aoMix(void *parameters) {
	extern int running; /* global running state */
	aoMixParam *para;
	int rate, i, tmp;
	float sample;
	/* buffer */
	char *buf;
	int bufLen;

	para = (aoMixParam *) parameters;

	bufLen = para->fmt->bits / 8 * para->fmt->channels * (rate = para->fmt->rate/250);
	buf = (char *) calloc(bufLen, sizeof(char));
	if(buf == NULL) {
		fprintf(stderr, "Error: Couldn't allocate audio buffer memory.\n");
		perror("calloc");
		pthread_exit(NULL);
	}

	while(running) { /* i hate globals */
		for (i=0; i<rate; i++) {

			sample = sumVoices() * 256; /* gain */

			/* clip the result
			 * this looks like a crude method, but works very well
			 */
			if (sample > 127) sample = 127;
			if (sample < -128) sample = -128;

			tmp = (int) sample;

			/* assuming 16bits */
			buf[4*i] = buf[4*i+2] = tmp & 0xff;
			buf[4*i+1] = buf[4*i+3] = (tmp >> 8) & 0xff;
		}

		if(ao_play(para->dev, buf, bufLen) == 0) running = 0;
	}
	pthread_exit(NULL);
}

void aoClose(ao_device *dev) {
	ao_close(dev);
	ao_shutdown();

}

