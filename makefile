CC      = gcc
RM      = rm -f

default: all

all: cube

cube: cube.c
	$(CC) -o cube cube.c
	touch mouse.txt

clean:
	$(RM) cube
	$(RM) mouse.txt