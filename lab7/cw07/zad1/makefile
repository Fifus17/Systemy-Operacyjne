saloon:
	gcc -Wall saloon.c sem.c shared_memory.c -o saloon

worker:
	gcc -Wall worker.c sem.c shared_memory.c -o worker

client:
	gcc -Wall client.c sem.c shared_memory.c -o client

all:
	make saloon
	make worker
	make client

clean:
	rm saloon client worker