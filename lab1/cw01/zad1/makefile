all:
	make lib_static
	make lib_shared

lib_static: libassignment1.c
	gcc -Wall -c libassignment1.c
	ar cr libassignment1.a libassignment1.o

lib_shared: libassignment1.c
	gcc -Wall -c -fPIC libassignment1.c
	gcc -Wall -shared -o libassignment1.so libassignment1.o

clean:
	rm -f *.o *.gch *.a *.so