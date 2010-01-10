CFLAGS=-Wall -Wextra -pedantic -std=c89
CFLAGS+=`pkg-config --cflags sdl`
#CFLAGS+=-ggdb3
#CC=clang

LIBS+=`pkg-config --libs sdl`

SRC+=audioout.c
SRC+=mixAudio.c

SRC+=configuration/routing.c
SRC+=configuration/example.c

SRC+=processing/misc.c
SRC+=processing/oscillators.c
SRC+=processing/tools.c
SRC+=processing/effects.c
SRC+=processing/voices.c
SRC+=processing/compute.c

SRC+=main.c

OBJS=$(SRC:.c=.o)

popcorn: $(OBJS)
	$(CC) $(CLFAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	rm -f $(OBJS) popcorn
