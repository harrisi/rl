CC=gcc
CFLAGS=-g
LDFLAGS=
LIBS=-lncurses
SOURCES=$(wildcard src/*.c)

rl: $(SOURCES)
	$(CC) -o bin/$@ $^ $(CFLAGS) $(LIBS) $(LDFLAGS)
