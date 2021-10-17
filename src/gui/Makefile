SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)

CC=gcc
CFLAGS=-ggdb -std=gnu17
LIBS=-lm -lSDL2 -lSDL2_image -lSDL2_ttf
AR=ar
ARFLAGS=rc

libgui.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	rm *.o libgui.a

