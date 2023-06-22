#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dataStructure.h"
#include "readIn.h"
#include "schedule.h"

int main(int argc, char** argv){
    // parse arguments
    arguments_t* arguments = createArguments();
    // get all processes, store in input list
    linkedList_t* processList = createLinkedList(); 
    if (readIn(argc, argv, arguments, processList) != 0) {
        return -1;
    }
    // run process
    schedule(processList, arguments); 
    //free memory
    free(arguments); 
    freeLinkedList_process(processList); 
    return 0; 
}