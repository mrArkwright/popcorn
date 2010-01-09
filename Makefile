#CC=clang

CFLAGS=-Wall -Wextra -pedantic -std=c89
CFLAGS+=-ggdb3
#JackOSX
#CFLAGS+=-I/usr/local/include
#LIBS+=-framework CoreAudio -framework CoreServices -framework AudioUnit -L/usr/local/lib -ljack
#*nix
#CFLAGS+=$(pkg-config --cflags jack)
#LIBS+=$(pkg-config --libs jack)


SRC+=jack.c
SRC+=voices.c
SRC+=oscillators.c
SRC+=effects.c
SRC+=main.c

OBJS=$(SRC:.c=.o)

popcorn: $(OBJS)
	$(CC) $(CLFAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	rm -f $(OBJS) popcorn
