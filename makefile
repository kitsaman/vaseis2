

all: execSort



execSort: main.o sort.o

	gcc -o execSort sort.o BF_64.a main.o -lm



main.o: main.c

	gcc -c -Wall main.c



sort.o: sort.c

	gcc -c -Wall sort.c



clean:
	
	rm -f execSort main.o sort.o
