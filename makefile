CFLAGS = -Wall -std=c99
PKG_CONFIG=`pkg-config allegro-5 allegro_font-5 allegro_image-5 --libs --cflags`

all: main.c states.o map.o sprites.o error_msg.o draw.o update.o game.o
	gcc main.c states.o map.o sprites.o error_msg.o draw.o update.o game.o -o boulder_dash $(PKG_CONFIG) $(CFLAGS)

debug: main.c states.o map.o sprites.o error_msg.o draw.o update.o game.o
	gcc -g main.c states.o map.o sprites.o error_msg.o draw.o update.o game.o -o boulder_dash $(PKG_CONFIG) $(CFLAGS)

states.o: states.c states.h map.c map.h error_msg.c error_msg.h sprites.c sprites.h draw.c draw.h update.c update.h game.c game.h
	gcc -c states.c $(CFLAGS)

map.o: map.c map.h
	gcc -c map.c $(CFLAGS)

sprites.o: sprites.c sprites.h error_msg.c error_msg.h map.c map.h
	gcc -c sprites.c $(CFLAGS)

error_msg.o: error_msg.c error_msg.h
	gcc -c error_msg.c $(CFLAGS)

draw.o: draw.c draw.h map.h map.c sprites.h sprites.c states.h states.c
	gcc -c draw.c $(CFLAGS)

update.o: update.c update.h map.c map.h
	gcc -c update.c $(CFLAGS)

game.o: game.c game.h map.c map.h
	gcc -c game.c $(CFLAGS)

clean:
	rm -f *.o

purge: clean
	rm -f boulder_dash
