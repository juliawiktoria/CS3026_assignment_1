# CS3026 Operating Systems - Assignment 1
# author: Julia Wiktoria Zieba
# ID: 51768280

# makefile for compiling the solution

all : shell.o mymemory.o
	gcc -o myprog shell.o mymemory.o 

shell.o : shell.c mymemory.h
	gcc -c shell.c

mymemory.o : mymemory.c mymemory.h
	gcc -c mymemory.c

clean:
	rm -f shell.o mymemory.o 
	
