#include "oscillators.h"

unsigned int sampleRate;

#define A 8
osc oscs[A];
osc lfos[A];

float m2pi = 2 * M_PI;

int range = 127;
float *globalOutput;


void oscRoute() { /* configuration (routing and params) */
	initOsc(&oscs[0], &oscRec, 440, 1, 0);
	initParam(&(oscs[0].freq), &(lfos[0].val), 10);
	initParam(&(oscs[0].param1), &(lfos[1].val), 0.9);
	initParam(&(oscs[0].vol), &(lfos[2].val), 0.9);

	initOsc(&lfos[0], &oscTri, 0.5, 1, 0);

	initOsc(&lfos[1], &oscSin, 0.4, 1, 0);

	initOsc(&lfos[2], &oscTri, 10, 1, -0.7);

	globalOutput = &(oscs[0].val);
}

void initOsc(osc *o, float (*func)(float, float, float), float freq, float vol, float param1) {
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

void initParam(param *p, float *mod, float range) {
	p->mod = mod;
	p->range = range;
}

void example_mixaudio(void *unused, Uint8 *stream, int len) {
	int i, j;
	float outputValue;

	for (i=0;i<len;i++) {
		
		for (j = 0; j < A; j++) {
			if (oscs[j].act == 1) {
				compOsc(&(oscs[j])); /* compute all active oscillators */
			}
		}

		for (j = 0; j < A; j++) {
			if (lfos[j].act == 1) {
				compOsc(&(lfos[j])); /* compute all active lfo's */
			}
		}

		outputValue = *globalOutput * range;

		if (outputValue > 127) outputValue = 127;        /* and clip the result */
		if (outputValue < -128) outputValue = -128;      /* this seems a crude method, but works very well */

		stream[i] = outputValue;
	}
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
	return p->val + ((p->mod != NULL) ? p->range * (*(p->mod)) : 0); 
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

