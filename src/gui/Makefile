SRC=$(wildcard src/gui/*.c)
OBJS=$(addprefix bin/, $(SRC:.c=.o))

CC=gcc
CFLAGS=-ggdb -std=gnu17
LIBS=-lm -lSDL2 -lSDL2_image -lSDL2_ttf
AR=ar
ARFLAGS=rc

bin/src/gui/%.o: src/gui/%.c src/gui/%.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

libgui.a: $(OBJS)
	$(AR) $(ARFLAGS) lib/$@ $(OBJS)

