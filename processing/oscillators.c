#include "oscillators.h"


/* --- Setup --- */

void setupOsc(osc *o) { /* set default static vaules for new osc */
	o->val = 0;
	o->phase = 0;
	o->func = &oscSin;

	o->freq.val = defValues + 2;
	setupParam(&(o->freq));

	o->vol.val = defValues + 1;
	setupParam(&(o->vol));

	o->param1.val = defValues + 0;
	setupParam(&(o->param1));
}


/* --- Computing --- */

void compOsc(osc *o) { /* compute current value of an osc */
	float freq, vol, param1;
	float spp; /* samples per period */

	/* compute params */
	freq = compParam(&(o->freq));
	vol = compParam(&(o->vol));
	param1 = compParam(&(o->param1));

	spp = sampleRate / freq; /* compute current freq */
	o->phase++;
	if (o->phase >= spp) o->phase -= spp; /* avoid wrap around of phase */

	o->val = o->func(o->phase, spp, param1) * vol; /* use specific osc-function (oscSin, oscTri etc) */
}

float oscSin(float phase, float spp, float param1) { /* compute sine-osc value TODO: lookup table or some kind of speedup */
	float out;

	param1 = param1 / 2 + 0.5;

	if (phase < spp * param1) { /* split sine-wave at a point, specified by param1 */
		out = sin(phase * twoPi / spp / (2 * param1)); /* readjust freq for first and second part of sine-wave */
	} else {
		phase += spp * (1 - 2 * param1);
		out = sin(phase * twoPi / spp / (2 - 2 * param1));
	}

	return out;
}

float oscTri(float phase, float spp, float param1) { /* compute triangle-osc value  */
	float out;

	param1 = param1 / 2 + 0.5;

	if (phase < spp * param1) {
		out = 2 * phase / spp / param1 - 1;
	} else {
		out = 1 - 2 * (phase - param1 * spp) / spp / (1 - param1);
	}

	return out;
}

float oscRec(float phase, float spp, float param1) { /* compute rectangle-osc value */
	float out;

	param1 = param1 / 2 + 0.5;

	out = (phase < spp * param1) ? 1 : -1; /* param1 specifies pulse-modulation */

	return out;
}
