#include <stdio.h>
#define BUFSIZE 50
static char buf[BUFSIZE];	/* buffer for ungetch */
static int bufp = 0;		/* next free position in buf */

static int getch(void);
static void ungetch(int);

typedef enum {KALL=1, HELP, QUIT, UNKWN} MOptions;

MOptions getmopt(void) {
	int c, opt;
	
	while((c = getch()) == ' ' || c == '\t'); /* skip whitespaces */

	if(c == EOF) return EOF;

	opt = c - '0';

	/* no menu point */
	if( opt < KALL || opt > QUIT) return UNKWN;
	
	return opt;
}

/* get a (possibly pushed-back) character */
static int getch(void) {
	return (bufp > 0) ? buf[--bufp] : getchar();
}

/* push character back on input */
static void ungetch(int c) {
	if(bufp >= BUFSIZE)
		fprintf(stderr, "ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
