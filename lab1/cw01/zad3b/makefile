SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall
LIB=libassignment2
REPL=repl.c
BINS=repl.c dll.h

all:
	make lib_static
	make lib_shared
	make exec_static
	make exec_shared
	make exec_dll

test:
	make clean
	make all
	export LD_LIBRARY_PATH=.
	mkdir -p results/
	make single_test

single_test:
	./repl_static.exe  < tests.txt > results/results_static_$(OPT_LV).txt
	./repl_shared.exe  < tests.txt > results/results_shared_$(OPT_LV).txt
	./repl_dll.exe < tests.txt > results/results_dll_$(OPT_LV).txt

opt_test:
	make clean
	make all
	export LD_LIBRARY_PATH=.
	mkdir -p results/
	make single_test OPT_LV="-O"
	make single_test OPT_LV="-O0"
	make single_test OPT_LV="-O1"
	make single_test OPT_LV="-O2"
	make single_test OPT_LV="-O3"
	make single_test OPT_LV="-Os"

lib_static: libassignment2.c
	gcc -Wall -c libassignment2.c
	ar cr libassignment2.a libassignment2.o

lib_shared: libassignment2.c
	gcc -Wall -c -fPIC libassignment2.c
	gcc -Wall -shared -o libassignment2.so libassignment2.o

exec_static: libassignment2.c
	gcc -Wall repl.c -o repl_static.exe -L . libassignment2.a

exec_shared: libassignment2.c
	gcc -Wall repl.c -o repl_shared.exe -L . libassignment2.so

exec_dll: libassignment2.c 
	gcc -Wall repl.c -o repl_dll.exe -ldl -D DLL

clean:
	rm -f *.o *.gch *.a *.so *.exe 

clean_txt:
	rm -f *.txt