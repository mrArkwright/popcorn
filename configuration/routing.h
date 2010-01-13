#ifndef ROUTING_H
#define ROUTING_H

#include "../processing/misc.h"
#include "../processing/oscillators.h"
#include "../processing/compute.h"



/* --- API --- */

/* - common */
void routeMasterOutput(unit *);
void routeVoicesOutput(unit *);

/* - Params */
void setParam(unit *, int, int, float);
void routeParam(unit *, int, int, unit *);

/* - Bools */
void setBool(unit *, int, char);
void routeBool(unit *, int, unit *);

/* - Oscillators */
unit *addOsc(int);



/* ---Helper ---*/

/* - common */
unit *addUnit(int);

/* - Params */
float *addGlobalParam();
float **getParamAddress(unit *, int, int, int);
char isGlobalParam(float **);
float *getValAddress(unit *, int);

/* - Bools */
char **getBoolParamAddress(unit *, int, int);
char *getBoolValAddress(unit *, int);

/* - Oscillators */
float (*getOscFunc(int type))(float, float, float);

#endif
