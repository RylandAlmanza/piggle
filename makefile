CC=gcc
LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -llua -lm
DEPS=*.h
main: $(DEPS) *.c
	$(CC) *.c -o main $(LIBS) -g
