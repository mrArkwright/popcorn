#include "oscillators.h"

float m2pi = 2 * M_PI;

void initOsc(osc *o, float (*func)(float, float, float), float *freq, float *vol, float *param1) {
	o->act = 1;
	o->val = 0;
	o->phase = 0;
	o->func = func;

	o->freq.val = freq;
	o->freq.mod = NULL;

	o->vol.val = vol;
	o->vol.mod = NULL;

	o->param1.val = param1;
	o->param1.mod = NULL;
}

void initParam(param *p, float *mod, float *range) {
	p->mod = mod;
	p->range = range;
}

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

float compParam(param *p) {
	/* this is where the routing magic happens */
	return *(p->val) + ((p->mod != NULL) ? *(p->range) * (*(p->mod)) : 0);
}

float oscSin(float phase, float spp, float param1) {
	float out;

	param1 = param1 / 2 + 0.5;

	if (phase < spp * param1) {
		out = sin(phase * m2pi / spp / (2 * param1));
	} else {
		phase += spp * (1 - 2 * param1);
		out = sin(phase * m2pi / spp / (2 - 2 * param1));
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

