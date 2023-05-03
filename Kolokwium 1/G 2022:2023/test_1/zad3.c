#include "zad3.h"
#include <stdio.h>

void readwrite(int pd, size_t block_size);

void createpipe(size_t block_size)
{
    int fd[2];
    pipe(fd);

    /* Odkomentuj poniższe funkcje zamieniając ... na deskryptory potoku */
    check_pipe(fd); 
    check_write(fd, block_size, readwrite);
}

void readwrite(int write_pd, size_t block_size)
{
    /* Otworz plik `unix.txt`, czytaj go po `block_size` bajtów
    i w takich `block_size` bajtowych kawałkach pisz do potoku `write_pd`.*/

    FILE* f = fopen("unix.txt", "w");
    char* buffer[block_size];
    int size = 0;

    while((size = fread(buffer, sizeof(char), block_size, f)) > 0){
        
        write(write_pd, buffer, size);
    }

    /* Zamknij plik */

    fclose(f);

}

int main()
{
    srand(42);
    size_t block_size = rand() % 128;
    createpipe(block_size);

    return 0;
}
