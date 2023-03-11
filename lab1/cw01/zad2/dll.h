#ifndef __DLL_H__
#define __DLL_H__

#ifdef DLL
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

char* DLLnames[] = {"createStructure", "initStructure", "readFromFile", "getFromStructure", "removeFromStructure", "clearStructure"}; // function names

void* DLLPointers[6]; // pointers to functions

void getDLLnames(char* filePath) { // get pointers to functions

    void *tmp = dlopen(filePath, RTLD_LAZY); // open dll

    if (tmp == NULL) {
        fprintf(stderr, "DLL NOT FOUND (%s)\n", filePath);
        return;
    }
    else {
    *(void **) (&createStructure) = dlsym(tmp,"createStructure");
    *(void **) (&initStructure) = dlsym(tmp,"initStructure");
    *(void **) (&readFromFile) = dlsym(tmp,"readFromFile");
    *(void **) (&getFromStructure) = dlsym(tmp,"getFromStructure");
    *(void **) (&removeFromStructure) = dlsym(tmp,"removeFromStructure");
    *(void **) (&clearStructure) = dlsym(tmp,"clearStructure");
    }
}

#else
void getDLLnames(char* filePath) {}
#endif
#endif