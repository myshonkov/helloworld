
# Makefile for Hello World project
#
hello: main.o hello.o
	gcc -o hello main.o -lX11 -L/usr/X11R6/lib

main.o: main.c
	gcc -c main.c


clean:
	rm -f *.o hello

