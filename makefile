CFLAGS = -Wall -std=c99
PKG_CONFIG=`pkg-config allegro-5 allegro_font-5 allegro_image-5 --libs --cflags`

all: main.c states.o map.o sprites.o error_msg.o draw.o
	gcc main.c states.o map.o sprites.o error_msg.o draw.o -o boulder_dash $(PKG_CONFIG) $(CFLAGS)

debug: main.c states.o map.o sprites.o error_msg.o draw.o
	gcc -g main.c states.o map.o sprites.o error_msg.o draw.o -o boulder_dash $(PKG_CONFIG) $(CFLAGS)

states.o: states.c states.h
	gcc -c states.c $(CFLAGS)

map.o: map.c map.h
	gcc -c map.c $(CFLAGS)

sprites.o: sprites.c sprites.h
	gcc -c sprites.c $(CFLAGS)

error_msg.o: error_msg.c error_msg.h
	gcc -c error_msg.c $(CFLAGS)

draw.o: draw.c draw.h
	gcc -c draw.c $(CFLAGS)

clean:
	rm -f *.o

purge: clean
	rm -f boulder_dash
