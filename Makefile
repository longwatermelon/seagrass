SRC=$(wildcard src/*.c)
OBJS=$(SRC:.c=.o)

CC=gcc
CFLAGS=-ggdb -std=gnu17
LIBS=-Lsrc/gui -lgui -lSDL2 -lSDL2_image -lSDL2_ttf -lm
SUBDIRS=src/gui

seagrass: $(OBJS)
	for i in $(SUBDIRS); do (cd $$i; $(MAKE)); done
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	for i in $(SUBDIRS); do (cd $$i; $(MAKE) clean); done
	rm src/*.o seagrass

