#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dlfcn.h>

int main (int l_param, char * wparam[]){
  int i;
  int tab[20]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};
/*
1) otworz biblioteke
2) przypisz wskaznikom f1 i f2 adresy funkcji z biblioteki sumuj i srednia
3) stworz Makefile kompilujacy biblioteke 'bibl1' ladowana dynamicznie
   oraz kompilujacy ten program
*/
  void *handle = dlopen("libbibl1.so", RTLD_LAZY);
  int (*f1)();
  f1 = (int (*)())dlsym(handle, "sumuj");
  double (*f2)();
  f2 = (double (*)())dlsym(handle, "srednia");

  for (i=0; i<5; i++) printf("Wynik: %d, %f\n", f1(tab+i, 20-i), f2(tab+i, 20-i));
  return 0;
}
