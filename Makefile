#CC=clang

CFLAGS=-Wall -Wextra -pedantic -std=c89
CFLAGS+=-ggdb3
#JackOSX
CFLAGS+=-I/usr/local/include
LIBS+=-framework CoreAudio -framework CoreServices -framework AudioUnit -L/usr/local/lib -ljack
#*nix
#CFLAGS+=$(pkg-config --cflags jack)
#LIBS+=$(pkg-config --libs jack)


SRC+=configuration/routing.c
SRC+=configuration/example.c

SRC+=processing/misc.c
SRC+=processing/oscillators.c
SRC+=processing/tools.c
SRC+=processing/effects.c
SRC+=processing/voices.c
SRC+=processing/compute.c

SRC+=jack.c
SRC+=main.c

OBJS=$(SRC:.c=.o)

popcorn: $(OBJS)
	$(CC) $(CLFAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	rm -f $(OBJS) popcorn
