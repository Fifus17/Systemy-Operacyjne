#ifdef DLL
    #include "libassignment2_so.h"
#else
    #include "libassignment2.h"
#endif

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>
#include <time.h>
#include <sys/times.h>
#include "dll.h"

// Environment variables
bool isRunning = false; // variable that indicates if the repl is running
bool structureInitialised = false; // variable that indicates if the structure has been initialised

// Structure
structure* structurePointer = NULL; // pointer to the structure

// Regex'es
regex_t regexINIT; // regex for the init command, id=0
regex_t regexCOUNT; // regex for the count command, id=1
regex_t regexGET; // regex for the show command, id=2
regex_t regexDELETE; // regex for the delete command, id=3
regex_t regexDESTROY; // regex for the destroy command, id=4
regex_t regexHELP; // regex for the help command, id=5
regex_t regexEXIT; // regex for the exit command, id=6

int cmdID = -1; // variable that stores the id of the current command
int cmdIndex = -1; // variable that stores given index of the command
char cmdFilePath[1024]; // variable that stores the path to the file

struct timespec rtstart, rtend; // variables that store the start and end time of the command (realtime)
struct tms tstart, tend; // variables that store the start and end time of the command (user and system time)

// Functions

void enableRegexes() {
    regcomp(&regexINIT, "init [1-9][0-9]*", REG_EXTENDED); // init regex
    regcomp(&regexCOUNT, "count ..*", REG_EXTENDED); // count regex
    regcomp(&regexGET, "get [0-9][0-9]*", REG_EXTENDED); // show regex
    regcomp(&regexDELETE, "delete [0-9][0-9]*", REG_EXTENDED); // delete regex
    regcomp(&regexDESTROY, "destroy", REG_EXTENDED); // destroy regex
    regcomp(&regexHELP, "help", REG_EXTENDED); // help regex
    regcomp(&regexEXIT, "exit", REG_EXTENDED); // exit regex
} // function that enables the regex'es

bool getCommand(char* command, size_t length) {
    if(regexec(&regexINIT, command, 0, NULL, 0) == 0) {
        cmdID = 0;
        sscanf(command, "init %d", &cmdIndex); // get the index from the command
        return true;
    }
    if (regexec(&regexCOUNT, command, 0, NULL, 0) == 0) {
        cmdID = 1;
        command += strlen("count "); // moving the pointer to the path, passing "count "
        memset(cmdFilePath, '\0', 1024);
        size_t pathLength = strcspn(command, "\n"); // get the end of the path
        strncpy(cmdFilePath, command, pathLength); // get the path to the file from the command
        return true;
    }
    if (regexec(&regexGET, command, 0, NULL, 0) == 0) {
        cmdID = 2;
        sscanf(command, "get %d", &cmdIndex);
        return true;
    }
    if(regexec(&regexDELETE, command, 0, NULL, 0) == 0) {
        cmdID = 3;
        sscanf(command, "delete %d", &cmdIndex);
        return true;
    }
    if(regexec(&regexDESTROY, command, 0, NULL, 0) == 0) {
        cmdID = 4;
        return true;
    }
    if(regexec(&regexHELP, command, 0, NULL, 0) == 0) {
        cmdID = 5;
        return true;
    }
    if(regexec(&regexEXIT, command, 0, NULL, 0) == 0) {
        cmdID = 6;
        return true;
    }
    cmdID = -1;
    return false;
} // function that gets the command from the user, returns true id the input is correct, otherwise false

void executeCommand() {
    switch (cmdID) {
        case 0:
            if(structureInitialised) {
                printf("Structure already initialised!\n");
                break;
            }
            initStructure(structurePointer ,cmdIndex);
            structureInitialised = true;
            break;
        case 1:
            if(!structureInitialised) {
                printf("Structure not initialised!\n");
                break;
            }
            readFromFile(structurePointer, cmdFilePath);
            break;
        case 2:
            if(!structureInitialised) {
                printf("Structure not initialised!\n");
                break;
            }
            char* show = getFromStructure(structurePointer, cmdIndex);
            printf("%s", show);
            break;
        case 3:
            if(!structureInitialised) {
                printf("Structure not initialised!\n");
                break;
            }
            removeFromStructure(structurePointer, cmdIndex);
            break;
        case 4:
            if(!structureInitialised) {
                printf("Structure not initialised!\n");
                break;
            }
            clearStructure(structurePointer);
            structureInitialised = false;
            break;
        case 5:
            printf("List of commands:\n");
            printf("init 'your input' - initialises the structure with the given size\n");
            printf("count 'file path' - counts the number of words in the given file\n");
            printf("get 'your input' - shows the word with the given index\n");
            printf("delete 'your input' - deletes the word with the given index\n");
            printf("destroy - destroys the structure\n");
            printf("exit - exits the program\n");
            printf("------------------------------------------------------------------\n");
            break;
        case 6:
            if(structureInitialised) {
                clearStructure(structurePointer);
            }
            isRunning = false;
            break;
        default:
            printf("Invalid command!\n");
            printf("Type 'help' to see the list of commands\n");
            break;
    }
} // function that executes given command

int main(int argc, char** argv) {
    getDLLnames("libassignment2.so"); // loading the library
    structurePointer = malloc(sizeof(structure)); // allocating memory for the structure
    enableRegexes(); // enable the regex'es
    isRunning = true; // set the repl to running
    while(isRunning) {
        printf(">>> ");
        
        // getting the input
        char* input = NULL;
        size_t length;
        getline(&input, &length, stdin);

        // parsing it into a command
        getCommand(input, length);

        // start of the time measurement
        clock_gettime(CLOCK_REALTIME, &rtstart); // real time start
        times(&tstart); // user and system time start

        // executing the command
        executeCommand();

        // end of the time measurement
        clock_gettime(CLOCK_REALTIME, &rtend); // real time end
        times(&tend); // user and system time end

        // printing the time
        if(cmdID >= 0 && cmdID <= 4) {
            printf("-------------------------\n");
            printf("Command execution times:\n");
            printf("Real time: %ld\n",rtend.tv_nsec - rtstart.tv_nsec);
            printf("User time: %ld\n",tend.tms_utime - tstart.tms_utime);
            printf("System time: %ld\n",tend.tms_stime - tstart.tms_stime);
            printf("-------------------------\n");
        }

        // freeing the memory
        free(input);
    }
    return 0;
}