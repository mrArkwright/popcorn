#include <stdio.h>

typedef enum {KALL=1, HELP, QUIT, UNKWN} MOptions;

int getmopt(void) {
	int c, opt;
	
	while((c = getchar()) == ' ' || c == '\t'); /* skip whitespaces */

	if(c == EOF) return EOF;

	opt = c - '0';

	/* no menu point */
	if( opt < KALL || opt > QUIT) return UNKWN;
	
	return opt;
}