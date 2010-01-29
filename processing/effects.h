#ifndef PROCESSING_EFFECTS_H
#define PROCESSING_EFFECTS_H

#include "misc.h"

typedef struct { /* type for lowpass-filter units */
	float val; /* current value of the filter */
	param cutoff, input; /* input and cutoff-freq of the filter TODO: input is no param */
} fxLowpass;

typedef struct { /* type for highpass-filter units */
	float val; /* current value of the filter */
	float tmp; /* last value for internal computing */
	param cutoff, input; /* input and cutoff-freq of the filter TODO: input is no param */
} fxHighpass;

typedef struct { /* type for bandpass-filter units */
	float val; /* current value of the fiter */
	float tmp; /* last value for internal computing */
	param freq, bandwidth, input; /* input, center-freq and bandwidth of the filter TODO: input is no param */
} fxBandpass;


/* --- Setup --- */

void setupFxLowpass(fxLowpass *);
void setupFxHighpass(fxHighpass *);
void setupFxBandpass(fxBandpass *);


/* --- Computing --- */

void compFxLowpass(fxLowpass *);
void compFxHighpass(fxHighpass *);
void compFxBandpass(fxBandpass *);

#endif
