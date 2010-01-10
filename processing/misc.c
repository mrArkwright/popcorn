#include "misc.h"

float **gParams = NULL;
int gParamCount = 0;
char gBools[2] = {0, 1};
float defParams[defParamCount] = {440, 1, 0};

float m2pi = 2 * M_PI;


/* --- Setup --- */

void setupParam(param *p) {
	p->mod = NULL;
	p->range = defParams + 1;
}


/* --- Computing --- */

float compParam(param *p) {
	/* this is where the routing magic happens */
	return *(p->val) + ((p->mod != NULL) ? *(p->range) * (*(p->mod)) : 0);
}


/* Stuff */

float getFreq(int note) {
	note -= 0;
	return 440 * pow(2, (note - 69) / 12.0);
}
