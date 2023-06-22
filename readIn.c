/* For parse command line arguments and readin processess*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "dataStructure.h"
#include "readIn.h"

//parse command line arguments
int parseArguments(int argc, char *argv[], arguments_t* arguments) {
    int c;
    while ((c = getopt(argc, argv, "f:s:m:q:")) != -1) {
        switch (c) {
            case 'f':
                arguments->filename = optarg;
                break;
            case 's':
                arguments->scheduler = optarg;
                break;
            case 'm':
                arguments->memoryAlgo = optarg;
                break;
            case 'q':
                arguments->quantum = optarg;
                break;
            case '?':
                if (optopt == 'f' || optopt == 's' || optopt == 'm' || optopt == 'q') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option -%c.\n", optopt);
                }
                return -1;
            default:
                abort();
        }
    }

    if (arguments->filename == NULL || arguments->scheduler == NULL || arguments->memoryAlgo == NULL || arguments->quantum == NULL) {
        fprintf(stderr, "Some options are not initialized\n");
        return -1;
    }
    return 0;
}

//read in all processes specified in txt
int readInProcess(char* filename, linkedList_t* processList){
    FILE* fp = fopen(filename, "r"); 
    
    if(fp == NULL){
        printf("can't open file\n");
        return -1; 
    }
    unsigned int arriveTime; 
    char name[9]; 
    unsigned int serviceTime; 
    int requiredMemory; 
    while (fscanf(fp, "%u %8s %u %d", &arriveTime, name, &serviceTime, &requiredMemory) == 4) {
        process_t* newProcess = createProcess(); 
        newProcess->arriveTime = arriveTime; 
        strcpy(newProcess->name, name); 
        newProcess->serviceTime = serviceTime;
        newProcess->serviceTimeRemain = serviceTime; 
        newProcess->requiredMemory = requiredMemory; 
        newProcess->assignedAt = -1; 
        newProcess->pid = -1; 
        node_t* newProcessNode = createNode(newProcess); 
        insertAtLinkedListTail(processList, newProcessNode); 
    }

    fclose(fp); 
    return 0; 
}

//main entry point, parse & readin processes
int readIn(int argc, char *argv[], arguments_t* arguments, linkedList_t* processList){
    if(parseArguments(argc, argv, arguments) != 0){
        return -1; 
    }
    if(readInProcess(arguments->filename, processList) != 0){
        return -1; 
    }
    return 0; 
}

