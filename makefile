CC=gcc
LIBS=-lSDL2 -lSDL2_image -llua
DEPS=*.h
main: $(DEPS) *.c
	$(CC) *.c -o main $(LIBS) -g