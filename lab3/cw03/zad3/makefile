
all:
	gcc -Wall assignment3.c -o assignment3.exe
	make test
	make clean

test:
	./assignment3.exe . test
	./assignment3.exe .. '#include'

clean:
	rm -f assignment3.exe