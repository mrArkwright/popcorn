#ifndef PROCESSING_TOOLS_H
#define PROCESSING_TOOLS_H

#include "misc.h"


typedef struct { /* type for 2 channler mixer units */
	float val; /* the current value of the mixer */
	char *act1, *act2; /* bools for setting the channels active or inactive */
	param vol1, vol2; /* volume values for the channles*/
	param input1, input2; /* input value of the channles TODO: is no param */
} mixer2ch;


/* --- Setup --- */

void setupMixer2ch(mixer2ch *);


/* --- Computing --- */

void compMixer2ch(mixer2ch *);

#endif
