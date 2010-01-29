#include "misc.h"

float **gValues = NULL; /* array which holds all static values. pointers in array -> locations don't change*/
int gValueCount = 0;
char gBools[2] = {0, 1}; /* only 2 static bool values are needed */
float defValues[defValueCount] = {0, 1, 440, 25}; /* some default static values for unit setup*/


/* --- Setup --- */

void setupParam(param *p) { /* set default static values for new param */
	p->mod = defValues + 0;
	p->range = defValues + 1;
}


/* --- Computing --- */

float compParam(param *p) { /* compute current value of param with its 3 values TODO: inline or some kind of speedup */
	return *(p->val) + *(p->range) * *(p->mod);
}


/* Stuff */

float getFreq(int note) { /* get freq in Hz from midi note value */
	return 440 * pow(2, (note - 69) / 12.0);
}
