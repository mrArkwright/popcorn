#include "misc.h"

void *ecMalloc(size_t size) {
	void *ptr;

	if ((ptr = malloc(size)) == NULL) {
		printf("memory allocation error!");
		exit(-1);
	}

	return ptr;
}

void *ecRealloc(void *ptr, size_t size) {
	void *ptrSav = ptr;

	if ((ptr = realloc(ptr, size)) == NULL) {
		printf("memory allocation error!");
		free(ptrSav);
		exit(-1);
	}

	return ptr;
}
