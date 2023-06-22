#ifndef CONTROL_REAL_PROCESS_H
#define CONTROL_REAL_PROCESS_H

#include "dataStructure.h"

int createRealProcess(process_t* process, unsigned int time);  
int terminateRealProcess(process_t* process, unsigned int time); 
int continueRealProcess(process_t* process, unsigned int time); 
int suspendRealProcess(process_t* process, unsigned int time); 
#endif