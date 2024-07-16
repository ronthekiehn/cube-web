CC      = gcc
RM      = rm -f

default: all

all: cube

cube: cube.c
	$(CC) -o cube cube.c

clean:
	$(RM) cube