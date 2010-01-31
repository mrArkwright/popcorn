#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "routing.h"
#include "hash.h"

#ifndef JSON_H
#define JSON_H
typedef enum {
	_ANALOG=1,
	_DIGITAL
} typus;
typedef struct{
	char* name;
	typus typ;
	paramType pt;
	paramOption po;
	boolType bt;
} OptionSet;

int routing();

#endif
