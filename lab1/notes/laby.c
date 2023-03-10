// mamy dwa rodzaje bibliotek, statyczne i dynamiczne
// biblioteka rozni sie tym ze zamiast main jest nazwa funkcji

#include<stdio.h>

void printMessage(){
    printf("Hello World From Static Library\n");
}

//  mozemy teraz wywolac ta funkcje bez includowania w innym pliku

// inny plik
int main(){
    printMessage();
    return 0;
}

// kompilacja do pliku obiektowego czyli rozszerzenie .o
// gcc -c laby.c

// teraz traktujemy fo archiwizerem ar
// ar cr liblaby.a laby.o
// musi miec przedrostek lib

// kropka to ze z tego folderu a l ze biblioteka
// gcc -o main main.c -L. -llaby

// jak zmienimy cokolwiek z bibliotece to musimy ja od nowa skompilowac i dolinkowac do pliku ktory z niej korzysta


// drugi rodzaj bibliotek to biblioteki dynamiczne
// kompilujemy biblitoeke z flaga -fPIC
// gcc -fPIC -c dynamiclibrary.c

// na podstawie tego pliku obiektowego produkujemy biblioteke .so (shared object)
// gcc -fPIC -shared -o libdynamiclibrary.so dynamiclibrary.o

// gcc -o main2.c -L. -ldynamiclibrary

// export LD_LIBRARY_PATH= ścieżka do biblioteki
// teraz powinno zadziałać