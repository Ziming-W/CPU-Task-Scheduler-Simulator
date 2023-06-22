#ifndef PRINT_RESULT_H
#define PRINT_RESULT_H

void printRunning(unsigned int time, char* processName, unsigned int remainTime); 
void printFinished(unsigned int time,char* processName, int procRemain); 
void printReady(unsigned int time, char* processName, int assignedAt);
void printStats(stats_t* stats, unsigned int time); 
void printFinishHash(unsigned int time, char* processName, char* hash); 
#endif