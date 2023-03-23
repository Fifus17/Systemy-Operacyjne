
all:
	gcc -Wall assignment2.c -o assignment2.exe
	make test
	make clean

test:
	./assignment2.exe emptyDirectory
	./assignment2.exe testingDirectory1

clean:
	rm -f assignment2.exe