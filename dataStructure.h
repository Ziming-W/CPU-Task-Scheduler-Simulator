#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#define MAX_MEMORY 2048

#include <unistd.h>
#include <sys/types.h>

// parse arguments
typedef struct arguments arguments_t; 

struct arguments{
    char* filename; 
    char* scheduler; 
    char* memoryAlgo; 
    char* quantum; 
};

arguments_t* createArguments();

// process files
typedef struct process process_t; 

struct process{
    unsigned int arriveTime; 
    char name[9]; 
    unsigned int serviceTimeRemain; 
    unsigned int serviceTime; 
    int requiredMemory; 
    int assignedAt; 
    pid_t pid; 
    int pipes_in[2]; 
    int pipes_out[2];
};

process_t* createProcess(); 

// memory node
typedef struct memoryChunk memoryChunk_t; 

struct memoryChunk{
    int start; 
    int size; 
}; 

memoryChunk_t* creatMemoryChunk(int start, int size);


// abstract linked list
typedef struct node node_t;

struct node {
    void* data;
    node_t* next;
};

typedef struct {
    node_t* head;
    node_t* tail;
    node_t* currRunning; 
} linkedList_t;

// linkedlist CRUD helpers 
node_t* createNode(void* data); 
linkedList_t* createLinkedList(); 
void insertAtLinkedListTail(linkedList_t* processList, node_t* newNode); 
void popFromLinkedList(linkedList_t* linkedList, node_t* node2Pop); 
int countLinkedListLength_general(linkedList_t* linkedList); 
int countLinkedListLength(linkedList_t* linkedList, unsigned int time); 
void printLinkedList_process(linkedList_t* linkedList); 
void insertAtIndex(linkedList_t* list, node_t* newNode, int index); 
void removeMemoryFromLinkedList(linkedList_t* linkedList, int start); 
void printLinkedList_memory(linkedList_t* linkedList); 

void freeLinkedList_process(linkedList_t* linkedList); 
void freeLinkedList_memory(linkedList_t* linkedList); 
void freeNode_process(node_t* node);
void freeNode_memory(node_t* node);  

int search4BestFitMemoryChunk(linkedList_t* linkedList, int requiredSize, int* start); 

// stats
typedef struct stats stats_t; 
struct stats{
    unsigned int sumTurnAround; 
    int numFinishedProcess; 
    double maxOverHead; 
    double sumOverHead; 
};

stats_t* createStats(); 

#endif