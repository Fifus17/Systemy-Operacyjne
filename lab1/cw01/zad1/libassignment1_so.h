#ifndef __LIBASSIGNMENT1_H__
#define __LIBASSIGNMENT1_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    size_t size;
    size_t currentSize;
    char** data; 
    bool* used;
} structure;

structure (*createStructure)(size_t size);

bool (*readFromFile)(structure* structure, char* filePath);

char* (*getFromStructure)(structure* structure, size_t index);

bool (*removeFromStructure)(structure* structure, size_t index);

void (*clearStructure)(structure* structure);

#endif