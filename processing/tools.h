#ifndef TOOLS_H
#define TOOLS_H

#include "misc.h"


typedef struct {
	float val;
	char *act1, *act2;
	param vol1, vol2;
	param input1, input2;
} mixer2ch;


/* --- Setup --- */

void setupMixer2ch(mixer2ch *);


/* --- Computing --- */

void compMixer2ch(mixer2ch *);

#endif
