#ifndef ROUTING_H
#define ROUTING_H

#include "../processing/misc.h"
#include "../processing/oscillators.h"
#include "../processing/compute.h"



/* --- Configuration API --- */

/* - common */
void routeMasterOutput(unit *src); /* routes masterOutput to output of src (src->scope must be usGLOBAL) */
void routeVoicesOutput(unit *src); /* routes voicesOutput to output of src (src->scope must be usLOCAL) */

/* - Params */
void setParam(unit *u, paramType type, paramOption option, float val); /* sets static value for param u->type.option to val */
void routeParam(unit *u, paramType type, paramOption option, unit *src); /* routes value for param u->type.option to output of src */

/* - Bools */
void setBool(unit *u, boolType type, char val); /* sets static value for bool u->type to val (0 or 1)*/
void routeBool(unit *u, boolType type, unit *src); /* routes value for bool u->type to output of src (output of src must be bool) */

/* - Oscillators */
unit *addOsc(unitScope scope); /* adds local (scope == usLOCAL) or global (scope == usGLOBAL) unit with type utOSC */
void setOscType(unit *u, oscType type); /* sets oscType of unit u to type (u->type must be utOSC) */



/* --- Helper --- */

/* - common */
unit *addUnit(unitScope);

/* - Params */
float *addGlobalParam();
float **getParamAddress(unit *, paramType, paramOption, int);
char isGlobalParam(float **);
float *getValAddress(unit *, int);

/* - Bools */
char **getBoolParamAddress(unit *, boolType, int);
char *getBoolValAddress(unit *, int);

/* - Oscillators */
float (*getOscFunc(oscType type))(float, float, float);

#endif
