ALLEGRO = `pkg-config allegro-5 allegro_font-5 --libs --cflags`

all: hello.c  
	gcc hello.c -o hello $(ALLEGRO)

purge:
	rm -f hello
