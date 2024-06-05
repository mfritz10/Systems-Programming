all: myprogram

myprogram: main.o libmyarray.a
	gcc main.o -L. -lmyarray -o myprogram

main.o: main.c
	gcc -Wall -c main.c -o main.o

libmyarray.a: myarray.c
	gcc -Wall -c myarray.c -o myarray.o
	ar rcs libmyarray.a myarray.o

myarray.o: myarray.c

clean: 
	rm -f *.o  *.a myprogram