#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#ifndef HASH_H
#define HASH_H

typedef unsigned char Hash;
typedef struct unitStruct {
		char id;
	} unit;
typedef struct le {
	Hash hash;
	void *dataPtr;
	char* name;

	struct le *next, *prev;
} ListElement;

Hash eightbitHash(char* string);

ListElement *addToHashlist( char* name, void *dataPtr,ListElement** hashList );

ListElement *getFromHashlist( char* name, ListElement** hashList);

void freeList(ListElement** hashList);

#endif

