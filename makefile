CFLAGS = -Wall -std=c99
PKG_CONFIG=`pkg-config allegro-5 allegro_font-5 --libs --cflags`

all: main.c
	gcc main.c -o boulder_dash $(PKG_CONFIG) $(CFLAGS)

purge:
	rm -f boulder_dash
