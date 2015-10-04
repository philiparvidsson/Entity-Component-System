CC = gcc
CFLAGS = -c -Wall -Os -Isource -Iinclude -D_DEBUG

LDFLAGS=
LDLIBS=-lm -lGL -lX11 -lGLEW

SOURCES=$(shell find source -type f -iname '*.c')
OBJECTS=$(foreach x, $(basename $(SOURCES)), $(x).o)

all: $(OBJECTS)
	mkdir -p bin
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o bin/sa14-game1

clean:
	rm -f $(TARGET) $(OBJECTS)

