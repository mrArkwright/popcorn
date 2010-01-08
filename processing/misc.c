#include "misc.h"

float m2pi = 2 * M_PI;

void initParam(param *p, float *mod, float *range) {
	p->mod = mod;
	p->range = range;
}

float compParam(param *p) {
	/* this is where the routing magic happens */
	return *(p->val) + ((p->mod != NULL) ? *(p->range) * (*(p->mod)) : 0);
}
