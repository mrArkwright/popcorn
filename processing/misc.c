#include "misc.h"

float **gParams = NULL;
int gParamCount = 0;
char gBools[2] = {0, 1};
float defParams[defParamCount] = {0, 1, 440};


/* --- Setup --- */

void setupParam(param *p) {
	p->mod = defParams + 0;
	p->range = defParams + 1;
}


/* --- Computing --- */

float compParam(param *p) {
	/* this is where the routing magic happens */
	return *(p->val) + *(p->range) * *(p->mod);
}


/* Stuff */

float getFreq(int note) {
	note -= 0;
	return 440 * pow(2, (note - 69) / 12.0);
}
