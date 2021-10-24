SRC=$(wildcard src/*.c)
OBJS=$(addprefix bin/, $(SRC:.c=.o))

CC=gcc
CFLAGS=-ggdb -std=gnu17
LIBS=-Llib -lgui -lSDL2 -lSDL2_image -lSDL2_ttf -lm
SUBDIRS=src/gui

all:
	mkdir -p bin/src/gui
	mkdir -p lib
	$(MAKE) -f src/gui/Makefile
	$(MAKE) seagrass

seagrass: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

bin/src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	rm -rf bin/*
	rm lib/*
	rm seagrass

