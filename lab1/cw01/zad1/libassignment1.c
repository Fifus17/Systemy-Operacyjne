#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    size_t size; // size of the array
    size_t currentSize; // current size of the array
    char** data; // pointer to the array of strings
    bool* used; // pointer to the array of bools that indicate if the string field is used
} structure;

char* wc = ""; // string that stores the wc command, for now left empty

// (1 task) Function that creates and returns a structure
structure createStructure(size_t size) {
    structure* newStructure = malloc(sizeof(structure)); // allocate memory for the structure
    return (structure) {
    newStructure->size = size, // set the size of the structure
    newStructure->currentSize = 0, // set the current size of the structure to 0, since it's newly created
    newStructure->data = calloc(size, sizeof(char*)), // allocate memory for the array of strings
    newStructure->used = calloc(size, sizeof(bool)) // allocate memory for the array of bools
    };
}

// (2 task) Function that runs wc on the file and saves the result in the structure. Returns true if the operation was successful, otherwise false.
bool readFromFile(structure* structure, char* filePath) {

    char tmpFilePath[] = "/tmp/XXXXXX"; // path to the temporary file
    int tmpFile = mkstemp(tmpFilePath); // create a temporary file, it must be created as an int
    if(tmpFile == -1) { // if the file couldn't be created
        fprintf(stderr, "Couldn't create a temporary file\n"); // print an error message
        return false;
    }

    snprintf(wc, 1024, "wc -l '%s' 1> '%s' 2>dev>null", filePath, tmpFilePath); // create the wc command
    system(wc); // run the wc command

    FILE* tmpFileOpened = fopen(tmpFilePath, "r"); // open the temporary file
    if(tmpFileOpened == 0) { // if the file doesn't exist
        fprintf(stderr, "Temporary file doesn't exist\n"); // print an error message
        fclose(tmpFileOpened); // close the file
        return false;
    }

    // checking the size of the file
    fseek(tmpFileOpened, 0, SEEK_END); // move the pointer to the end of the file
    size_t fileSize = ftell(tmpFileOpened); // get the position of the pointer
    fseek(tmpFileOpened, 0, SEEK_SET); // move the pointer to the beginning of the file
    char* content = calloc(fileSize, sizeof(char)); // allocate memory for the content of the file
    fread(content, sizeof(char), fileSize, tmpFileOpened); // read the content of the file
    fclose(tmpFileOpened); // close the temporary file

    if(strlen(content) == 0) { // if the file is empty
        fprintf(stderr, "File is empty\n"); // print an error message
        free(content); // free the memory
        return false;
    }

    // saving data in the structure
    if(structure->currentSize != structure->size) { // if the structure is full
        structure->data[structure->currentSize] = content; // save the content of the file in the structure
        structure->used[structure->currentSize] = true; // set the field to true, since it's used
        structure->currentSize++; // increase the current size of the structure
    }
    else { // if the structure is full
        fprintf(stderr, "Structure is full\n"); // print an error message
        free(content); // free the memory
        return false;
    }

    // removing the remporary file
    snprintf(wc, 1024, "rm -f '%s'", tmpFilePath); // create the command to remove the temporary file
    system(wc); // run the command
    return true; // return true, since the operation was successful
}

// (3 task) Function that returns the content of the structure at the given index.
char* getFromStructure(structure* structure, size_t index) {
    if(index < structure->currentSize) { // if the index is valid
        return structure->data[index]; // return the content of the structure at the given index
    }
    else { // if the index is invalid
        fprintf(stderr, "Index out of bounds\n"); // print an error message
        return NULL; // return NULL
    }
}

// (4 task) Function that removes the content of the structure at the given index.
bool removeFromStructure(structure* structure, size_t index) {
    if(index < structure->currentSize) { // if the index is valid
        free(structure->data[index]); // free the memory
        structure->used[index] = false; // set the field to false, since it's not used
        structure->currentSize--; // decrease the current size of the structure
        return true; // return true, since the operation was successful
    }

    fprintf(stderr, "Index out of bounds\n"); // print an error message
    return false; // return false, since the operation was unsuccessful
}

// (5 task) Function that frees the memory allocated for the structure.
void clearStructure(structure* structure) {
    for(size_t i = 0; i < structure->currentSize; i++) { // for each element in the structure
        if(structure->used[i] == true) {
        free(structure->data[i]); // free the memory
        structure->used[i] = false; // set the field to false, since it's not used anymore
        }
    }
}
