// Napisz program, w ktorym stworzysz n watkow (parametr programu) w ktorym kazdy w nieskonczonej petli wypisuje na konsole napis typu "Hello tu watek n. Iteracja m". Dodatkowo po stworzeniu wszystkich watkow, program glowny (takze w nieskonczonej petli) wyspiuje napis typu "Hello tu program glowny. Iteracja m".

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void* hello(void* arg) {
    int m = 0;
    pthread_t n = pthread_self();
    while (1) {
        sleep(1);
        m++;
        printf("Hello, tu watek %lu. Iteracja %d\n. ", (unsigned long)n, m);
    }
}

int main(int argc, char** argv) {
    int n = atoi(argv[1]);
    int* arg = calloc(n, sizeof(int));

    pthread_t *tid = calloc(n, sizeof(pthread_t));

    for(int i = 0; i < n; i++) {
        arg[i] = i;
        pthread_create(&tid[i], NULL, hello, &arg[i]);
    }

    int m = 0;
    while (m < 10) {
        usleep(1000000);
        m++;
        printf("Hello, tu program glowny. Iteracja %d\n", m);
    }
    // pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    for (int i = 0; i < n; i++) {
        pthread_cancel(tid[i]);
    }
}


// Zmodyfikuj poprzednie w taki sposob ze program glowny "zabija" w sposob asynchroniczny wszystkie watki po wypisaniu na konsole swojego napisu 10 razy

// Zmodyfikuj poprzednie: W warku musisz miec dwa miejsca drukowania komunikatu na konsole, nastepnie proggram glowny zabija swoje watki w sposob synchroniczny po wypisaniu 10 razy swojego komunikatu. Ustaw miejsce synchronizacji na miejsce pomiedzy wypisaniem pierwszego i drugiego komunikatu na konsole.

// napisz program w ktorym utworzysz watek, ltory wypisze na konsole napis tpyu "Hello tu watek iteracja m" (m parametr programu) i "sie zakonczy". program glowny powinien "zaczekac" na zakonczenie sie swojego watku, "odebrac" wartosc zwrocona przez watek i wypisac po tym potwierdzenie typu "watek zakonczony. konczymy. wartosc zwrocona przez watek to: (wartosc)"