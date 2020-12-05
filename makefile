CFLAGS = -Wall -std=c99
PKG_CONFIG=`pkg-config allegro-5 allegro_font-5 --libs --cflags`

all: main.c states.o
	gcc main.c states.o -o boulder_dash $(PKG_CONFIG) $(CFLAGS)

states.o: states.c states.h
	gcc -c states.c $(CFLAGS)

clean:
	rm -f *.o

purge: clean
	rm -f boulder_dash
