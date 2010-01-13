#include "oscillators.h"


/* --- Setup --- */

void setupOsc(osc *o) {
	o->phase = 0;
	o->func = &oscSin;

	o->freq.val = defParams + 0;
	setupParam(&(o->freq));

	o->vol.val = defParams + 1;
	setupParam(&(o->vol));

	o->param1.val = defParams + 2;
	setupParam(&(o->param1));
}


/* --- Computing --- */

void compOsc(osc *o) {
	float freq, vol, param1;
	float spp; /* samples per period */

	freq = compParam(&(o->freq));
	vol = compParam(&(o->vol));
	param1 = compParam(&(o->param1));

	spp = sampleRate / freq;
	o->phase++;
	if (o->phase >= spp) o->phase -= spp;

	o->val = o->func(o->phase, spp, param1) * vol;
}

float oscSin(float phase, float spp, float param1) {
	float out;

	param1 = param1 / 2 + 0.5;

	if (phase < spp * param1) {
		out = sin(phase * 2*M_PI / spp / (2 * param1));
	} else {
		phase += spp * (1 - 2 * param1);
		out = sin(phase * 2*M_PI / spp / (2 - 2 * param1));
	}

	return out;
}

float oscTri(float phase, float spp, float param1) {
	float out;

	param1 = param1 / 2 + 0.5;

	if (phase < spp * param1) {
		out = 2 * phase / spp / param1 - 1;
	} else {
		out = 1 - 2 * (phase - param1 * spp) / spp / (1 - param1);
	}

	return out;
}

float oscRec(float phase, float spp, float param1) {
	float out;

	param1 = param1 / 2 + 0.5;

	out = (phase < spp * param1) ? 1 : -1;

	return out;
}
