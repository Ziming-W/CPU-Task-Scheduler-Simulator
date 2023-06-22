/*helpers for printing to stdout*/

#include<stdio.h>
#include "dataStructure.h"
#include<math.h>

void printRunning(unsigned int time, char* processName, unsigned int remainTime){
    printf("%u,RUNNING,process_name=%s,remaining_time=%d\n", time, processName, remainTime);
}

void printFinished(unsigned int time,char* processName, int procRemain){
    printf("%u,FINISHED,process_name=%s,proc_remaining=%d\n", time, processName, procRemain);
}

void printReady(unsigned int time, char* processName, int assignedAt){
    printf("%u,READY,process_name=%s,assigned_at=%d\n", time, processName, assignedAt); 
}

void printStats(stats_t* stats, unsigned int time){
    printf("Turnaround time %u\n", (unsigned int)ceil((double)stats->sumTurnAround/(stats->numFinishedProcess))); 
    printf("Time overhead %.2f %.2f\n", round(stats->maxOverHead*100)/100, round(((double)(stats->sumOverHead) / stats->numFinishedProcess)*100)/100); 
    printf("Makespan %u\n", time); 
}

void printFinishHash(unsigned int time, char* processName, char* hash){
    printf("%d,FINISHED-PROCESS,process_name=%s,sha=%s\n", time, processName, hash); 
}