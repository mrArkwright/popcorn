#include "misc.h"

float m2pi = 2 * M_PI;


/* --- Initialization --- */

void initParam(param *p, float *mod, float *range) {
	p->mod = mod;
	p->range = range;
}


/* --- Computing --- */

float compParam(param *p) {
	/* this is where the routing magic happens */
	return *(p->val) + ((p->mod != NULL) ? *(p->range) * (*(p->mod)) : 0);
}


/* Stuff */

float getFreq(int note) {
	note -= 12;
	return 440 * pow(2, (note - 69) / 12.0);
}
