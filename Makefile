CFLAGS=-Wall -Wextra -pedantic -std=c89
CFLAGS+=`pkg-config --cflags sdl`
#CFLAGS+=-ggdb3
#CC=clang

LIBS+=`pkg-config --libs sdl`

SRC+=audioout.c
SRC+=oscillators.c
SRC+=main.c

OBJS=$(SRC:.c=.o)

popcorn: $(OBJS)
	$(CC) $(CLFAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	rm -f $(OBJS) popcorn
