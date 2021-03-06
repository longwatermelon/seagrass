SRC=$(wildcard src/*.c)
OBJS=$(addprefix obj/, $(SRC:.c=.o))

CC=gcc
CFLAGS=-ggdb -std=gnu17 -Wall -Werror
LIBS=-Llib -lgui -lSDL2 -lSDL2_image -lSDL2_ttf -lm
SUBDIRS=src/gui

all:
	mkdir -p obj/src/gui
	mkdir -p lib
	$(MAKE) -f src/gui/Makefile
	$(MAKE) seagrass

seagrass: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

obj/src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	rm -rf obj lib
	rm seagrass

install:
	cp seagrass /usr/bin
	mkdir /usr/share/seagrass
	cp -r res /usr/share/seagrass/res

uninstall:
	rm /usr/bin/seagrass
	rm -rf /usr/share/seagrass

