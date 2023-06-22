/* Simulate CPU cycles*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "schedule.h"
#include "dataStructure.h"
#include "printResult.h"
#include "controlRealProcess.h"

//as spec required
#define IMPLEMENTS_REAL_PROCESS

/*[Used by SJF]Select the shortest job node from readyQueue*/
node_t* selectSJF(linkedList_t* readyQueue){
    node_t* currNode = readyQueue->head; 
    node_t* minNode = currNode; 
    while(currNode != NULL){
        process_t* currProcess = ((process_t*)currNode->data);
        process_t* minProcess =  ((process_t*)minNode->data); 
        if(currProcess->serviceTimeRemain < minProcess->serviceTimeRemain){
            minNode = currNode; 
        }
        else if(currProcess->serviceTimeRemain == minProcess->serviceTimeRemain && currProcess->arriveTime == minProcess->arriveTime){ 
            if(strcmp(currProcess->name, minProcess->name) < 0){
                minNode = currNode; 
            }
        }
        currNode = currNode->next; 
    }
    return minNode; 
}

/*[Used by best-fit] select best-fit memory allocation from memory allocation list*/
int allocateMemory(linkedList_t* inputQueue, linkedList_t* readyQueue, node_t* candidate, linkedList_t* memoryAllocationList, unsigned int time){
    //printLinkedList_memory(memoryAllocationList); 
    process_t* candidateProcess = (process_t*)(candidate->data);     
    int start = -1; 
    int location = search4BestFitMemoryChunk(memoryAllocationList, candidateProcess->requiredMemory, &start);
    //can't fit any more
    if(location == -1){
        return -1; 
    }
    memoryChunk_t* newMemoryChunk = creatMemoryChunk(start, candidateProcess->requiredMemory);
    node_t* newNode = createNode(newMemoryChunk); 
    insertAtIndex(memoryAllocationList, newNode, location); 
    //enqueue
    candidateProcess->assignedAt = start; 
    popFromLinkedList(inputQueue, candidate); 
    insertAtLinkedListTail(readyQueue, candidate); 
    printReady(time, candidateProcess->name, start); 
    return 0; 
}

/*[Used by SJF/RR] try to assign memory and enqueue elements from input queue to readyQueue*/
void assignMemory_enqueueInput(linkedList_t* inputQueue, linkedList_t* readyQueue, linkedList_t* memoryAllocationList, arguments_t* arguments, unsigned int time){
    //if infinite memory, simply move all processes that has available arriveTime to readyQueue
    if(strcmp(arguments->memoryAlgo, "infinite") == 0){
        node_t* curr = inputQueue->head;  
        while(curr != NULL){
            if(((process_t*)curr->data)->arriveTime <= time){
                popFromLinkedList(inputQueue, curr); 
                insertAtLinkedListTail(readyQueue, curr); 
                curr = inputQueue->head; 
            }
            else{
                curr = curr->next; 
            }
        }
    }
    //if memory is limited. use best fit to iteratively allocate memory
    else if(strcmp(arguments->memoryAlgo, "best-fit") == 0){
        node_t* curr = inputQueue->head;  
        while(curr != NULL){
            if(((process_t*)curr->data)->arriveTime <= time){
                if(allocateMemory(inputQueue, readyQueue, curr, memoryAllocationList, time) == 0){
                    curr = inputQueue->head; 
                }
                else{
                    curr = curr->next; 
                }
            }
            else{
                curr = curr->next; 
            }
        }
    }
    return; 
}

/* [Used by all]After a process finishes, update stats including turnaround time etc*/
void updataStatsAfterProcessFinish(unsigned int time, process_t* process, stats_t* stats){
    unsigned int turnAround = (time - process->arriveTime); 
    double timeOverHead = (double)turnAround / (process->serviceTime);
    double roundedOverHead = round(timeOverHead * 100) / 100;
    stats->numFinishedProcess ++; 
    stats->sumTurnAround += turnAround; 
    stats->sumOverHead += timeOverHead; 
    stats->maxOverHead = (stats->maxOverHead < roundedOverHead) ? roundedOverHead : stats->maxOverHead; 
}

/*[Used by SJF] check if a SJF process has finished, if yes, finish it*/
void checkFinished_SJF(int quantum, linkedList_t* inputQueue, linkedList_t* readyQueue, linkedList_t* memoryAllocationList, arguments_t* arguments, unsigned int* timer, stats_t* stats){
    //if currRunning process is finished
    if(readyQueue->currRunning != NULL && ((process_t*)readyQueue->currRunning->data)->serviceTimeRemain == 0){
        printFinished(*timer, ((process_t*)readyQueue->currRunning->data)->name, countLinkedListLength(readyQueue, *timer) - 1);
        terminateRealProcess(((process_t*)readyQueue->currRunning->data), *timer); 
        updataStatsAfterProcessFinish(*timer, (process_t*)readyQueue->currRunning->data, stats); 
        //remove from ready queue
        popFromLinkedList(readyQueue, readyQueue->currRunning);
        //free simulated memory
        if(strcmp(arguments->memoryAlgo, "best-fit") == 0){
            removeMemoryFromLinkedList(memoryAllocationList, ((process_t*)readyQueue->currRunning->data)->assignedAt); 
        }
        // free real memory
        freeNode_process(readyQueue->currRunning); 
        readyQueue->currRunning = NULL;
    }
    // if currRunning is not finished
    else if(readyQueue->currRunning != NULL && ((process_t*)readyQueue->currRunning->data)->serviceTimeRemain > 0){
        continueRealProcess(((process_t*)readyQueue->currRunning->data), *timer); 
    }
}

/* [used by SJF] select a SJF process and RUN*/
void selectNRun_SJF(int quantum, linkedList_t* inputQueue, linkedList_t* readyQueue, linkedList_t* memoryAllocationList, arguments_t* arguments, unsigned int* timer, stats_t* stats){
    // if curr running process is finished or haven't been initialized, select and pop new min node
    if(readyQueue->currRunning == NULL){
        readyQueue->currRunning = selectSJF(readyQueue); 
        //real process
        createRealProcess(((process_t*)readyQueue->currRunning->data), *timer); 
        printRunning(*timer, ((process_t*)readyQueue->currRunning->data)->name, ((process_t*)readyQueue->currRunning->data)->serviceTimeRemain);
    }
    //update serviceTime for the process that is running
    //make sure servieTimeRemain doesn't go below 0 (since its unsigned int)
    if(((process_t*)readyQueue->currRunning->data)->serviceTimeRemain <= quantum){
        ((process_t*)readyQueue->currRunning->data)->serviceTimeRemain = 0; 
    }
    else{
       ((process_t*)readyQueue->currRunning->data)->serviceTimeRemain -= quantum;
    }
}

/*[Used by RR] check if a RR process has finished, if yes, finish it*/
/*if not, with hold value, add to tail later after enqueue new ready processes*/
void checkFinished_RR(int quantum, linkedList_t* inputQueue, linkedList_t* readyQueue, linkedList_t* memoryAllocationList, arguments_t* arguments, unsigned int* timer, stats_t* stats, node_t** pointer_2_rrOldHeadUnfinished){
    //if last run process is finished, remove this process
    if(readyQueue->head != NULL && ((process_t*)readyQueue->head->data)->serviceTimeRemain == 0){
        printFinished(*timer, ((process_t*)readyQueue->head->data)->name, countLinkedListLength(readyQueue, *timer) + countLinkedListLength(inputQueue, *timer) - 1);
        terminateRealProcess(((process_t*)readyQueue->head->data), *timer); 
        updataStatsAfterProcessFinish(*timer, (process_t*)readyQueue->head->data, stats); 
        //free simulated memory
        if(strcmp(arguments->memoryAlgo, "best-fit") == 0){
            removeMemoryFromLinkedList(memoryAllocationList, ((process_t*)readyQueue->head->data)->assignedAt); 
        }
        //remove from ready queue
        node_t* rrOldHeadFinished = readyQueue->head; 
        popFromLinkedList(readyQueue, readyQueue->head);
        freeNode_process(rrOldHeadFinished); 
    }
    // else, withhold the value, add to tail after memory allocataion
    else if(readyQueue->head != NULL && ((process_t*)readyQueue->head->data)->serviceTimeRemain > 0){
        *pointer_2_rrOldHeadUnfinished = readyQueue->head; 
        popFromLinkedList(readyQueue, readyQueue->head);
    }
}

/* [used by RR] select next process in readyQueue and RUN*/
void selectNRun_RR(int quantum, linkedList_t* inputQueue, linkedList_t* readyQueue, linkedList_t* memoryAllocationList, arguments_t* arguments, unsigned int* timer, stats_t* stats, node_t** pointer_2_rrOldHeadUnfinished, char* rrLastRunProcessName){
    if(readyQueue->head != NULL ){
        //for rr, if there is only one process in the list, it can keep running, so don't printRunning in this case
        // if CPU just started or continue running the only process in the list
        if(rrLastRunProcessName[0] == 0 || strcmp(((process_t*)readyQueue->head->data)->name, rrLastRunProcessName) != 0){
            // if old process has been popped, and new process is added to front, we need to suspend old process
            if(rrLastRunProcessName[0] != 0 && strcmp(((process_t*)readyQueue->head->data)->name, rrLastRunProcessName) != 0){
                if(*pointer_2_rrOldHeadUnfinished != NULL){
                    suspendRealProcess((process_t*)((*pointer_2_rrOldHeadUnfinished)->data), *timer); 
                }
            }
            strcpy(rrLastRunProcessName, ((process_t*)readyQueue->head->data)->name);
            printRunning(*timer, ((process_t*)readyQueue->head->data)->name, ((process_t*)readyQueue->head->data)->serviceTimeRemain);
            //when running a process, decide whether to create a new process or continue running old process
            if(((process_t*)readyQueue->head->data)->serviceTimeRemain == ((process_t*)readyQueue->head->data)->serviceTime){
                //if we have a brand new process
                createRealProcess(((process_t*)readyQueue->head->data), *timer); 
            }
            else{
                //if we have to continue a old process
                continueRealProcess(((process_t*)readyQueue->head->data), *timer); 
            }
        }
        //else, in this case, there is still only one process in the readyQueue after memory allocation, we can continue to run last popped process
        else{
            continueRealProcess(((process_t*)readyQueue->head->data), *timer); 
        }
        //update serviceTime for the process that is running
        // make sure servieTimeRemain doesn't go below 0 (since its unsigned int)
        if(((process_t*)readyQueue->head->data)->serviceTimeRemain <= quantum){
            ((process_t*)readyQueue->head->data)->serviceTimeRemain = 0; 
        }
        else{
           ((process_t*)readyQueue->head->data)->serviceTimeRemain -= quantum;
        }
    }
}

/*Simulate a cycle*/
int runCycle(linkedList_t* inputQueue, linkedList_t* readyQueue, linkedList_t* memoryAllocationList, arguments_t* arguments, unsigned int* timer, char* rrLastRunProcessName, stats_t* stats){
    int quantum = atoi(arguments->quantum); 
    node_t* rrOldHeadUnfinished = NULL; // [used by RR]store the popped node from beginning, add to tail after memory allocation
    /******Check if process run in last quantum is finished*****/
    //SJF
    if(strcmp(arguments->scheduler, "SJF") == 0){
        checkFinished_SJF(quantum, inputQueue, readyQueue, memoryAllocationList, arguments, timer,stats); 
    }
    //RR
    else if(strcmp(arguments->scheduler, "RR") == 0){
        checkFinished_RR(quantum, inputQueue, readyQueue, memoryAllocationList, arguments, timer, stats, &rrOldHeadUnfinished); 
    }

    /******if both input queue and ready queue is null, return 0 to terminate the process*****/
    if(inputQueue->head == NULL && readyQueue->head == NULL && rrOldHeadUnfinished == NULL){
        return 0; 
    }

    /****** try to assign memory and enqueue input*****/
    if(inputQueue->head != NULL){
        assignMemory_enqueueInput(inputQueue, readyQueue, memoryAllocationList, arguments, *timer); 
    }

    /*****(this may not happen according to the spec)if after assigning, still empty readyQueue, but there are elements in Input, go IDLE and wait*****/
    if(inputQueue->head != NULL && readyQueue->head == NULL && rrOldHeadUnfinished == NULL){
        *timer += quantum; 
        return 1;
    }

    /*****[used by RR] If round robin has popped an element, add to end now*****/
    if(strcmp(arguments->scheduler, "RR") == 0 && rrOldHeadUnfinished != NULL){
        insertAtLinkedListTail(readyQueue, rrOldHeadUnfinished); 
    }

    /*****run processes in this cycle*****/
    //SJF
    if(strcmp(arguments->scheduler, "SJF") == 0){
        selectNRun_SJF(quantum, inputQueue, readyQueue, memoryAllocationList, arguments, timer, stats); 
    }
    // RR
    else if(strcmp(arguments->scheduler, "RR") == 0){
        selectNRun_RR(quantum, inputQueue, readyQueue, memoryAllocationList, arguments, timer, stats, &rrOldHeadUnfinished, rrLastRunProcessName); 
    }
    /*****update timer*****/
    *timer += quantum; 
    return 1;
}

/*****Main Entry Point*****/
void schedule(linkedList_t* inputQueue, arguments_t* arguments){
    //initial arguments
    linkedList_t* readyQueue = createLinkedList(); 
    linkedList_t* memoryAllocationList = createLinkedList();
    unsigned int timer = 0; 
    char rrLastRunProcessName[9] = "";
    stats_t* stats = createStats();
    //run cycle 
    while(1){
        if(runCycle(inputQueue, readyQueue, memoryAllocationList, arguments, &timer, rrLastRunProcessName, stats) == 0){
            break; 
        } 
    }
    //print final stats
    printStats(stats, timer); 
    //free memory
    freeLinkedList_process(readyQueue); 
    freeLinkedList_memory(memoryAllocationList); 
    free(stats); 
}



