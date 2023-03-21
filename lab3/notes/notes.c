#include <stdio.h>
#include <unistd.h> 

int main(){
    printf("Poczatek\n");
    fork();
    printf("Koniec\n");
    return 0;
}