#include "misc.h"

float **gParams = NULL; /* all static values are saved in this array TODO: double pointer not needed!, rename to gValues! */
int gParamCount = 0;
char gBools[2] = {0, 1}; /* only 2 static bool values are needed */
float defParams[defParamCount] = {0, 1, 440, 25}; /* some default static values for unit setup */


/* --- Setup --- */

void setupParam(param *p) { /* set default static values for new param */
	p->mod = defParams + 0;
	p->range = defParams + 1;
}


/* --- Computing --- */

float compParam(param *p) { /* compute current value of param with its 3 values TODO: inline ore some kind of speedup*/
	return *(p->val) + *(p->range) * *(p->mod);
}


/* Stuff */

float getFreq(int note) { /* get freq in Hz from midi note value */
	return 440 * pow(2, (note - 69) / 12.0);
}
