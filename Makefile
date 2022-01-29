CC = gcc
FLAGS = -Wall -ansi -g
EXS = main
OBJ = main.o minimiza.o

all: clean main

$(EXS): %: %.c
	$(CC) $(FLAGS) -c main.c
	$(CC) $(FLAGS) -c minimiza.c
	$(CC) $(FLAGS) -o main main.o minimiza.o -I -l ./lib_afnd_64.a


clean:
	rm -rf $(EXS) $(OBJ) 