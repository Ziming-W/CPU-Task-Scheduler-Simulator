/* Data structures */

#include <stdlib.h>
#include <assert.h>
#include "dataStructure.h"
#include<stdio.h>

// arguments
arguments_t* createArguments() {
    arguments_t* arguments = malloc(sizeof(*arguments));
    assert(arguments != NULL); 
    return arguments;  
}

// process
process_t* createProcess(){
    process_t* process = (process_t*)malloc(sizeof(*process)); 
    assert(process != NULL); 
    return process; 
}

// memory chunk
memoryChunk_t* creatMemoryChunk(int start, int size){
    memoryChunk_t* memoryChunk = (memoryChunk_t*)malloc(sizeof(*memoryChunk)); 
    assert(memoryChunk != NULL); 
    memoryChunk->start = start; 
    memoryChunk->size = size; 
    return memoryChunk; 
}


// process list
node_t* createNode(void* data){
    node_t* node = malloc(sizeof(node_t)); 
    assert(node != NULL); 
    node->data = data; 
    node->next = NULL; 
    return node; 
}

linkedList_t* createLinkedList(){
    linkedList_t* linkedList = (linkedList_t*)malloc(sizeof(linkedList_t));
    assert(linkedList != NULL); 
    linkedList->head = NULL; 
    linkedList->tail = NULL; 
    linkedList->currRunning = NULL; 
    return linkedList;  
}

int removeFromLinkedListStart(linkedList_t* linkedList){
    assert(linkedList != NULL); 
    if(!linkedList->head){
        return -1; 
    }
    node_t* oldHead = linkedList->head; 
    linkedList->head = linkedList->head->next; 
    
    free(oldHead); 
    if(linkedList->head == NULL){
        linkedList->tail = NULL; 
    }
    return 0; 
}

void popFromLinkedListStart(linkedList_t* linkedList){
    assert(linkedList != NULL && linkedList->head != NULL); 
    linkedList->head = linkedList->head->next; 
    if(linkedList->head == NULL){
        linkedList->tail = NULL; 
    }
    return;  
}

void popFromLinkedList(linkedList_t* linkedList, node_t* node2Pop) {
    if (linkedList->head == NULL || node2Pop == NULL) {
        // List is empty or node2Pop is invalid
        printf("invalid llist"); 
        return;
    }
    if (node2Pop == linkedList->head) {
        // Pop the head node
        popFromLinkedListStart(linkedList);
        return; 
    }
    node_t* prev = NULL;
    node_t* curr = linkedList->head;
    while (curr != NULL && curr != node2Pop) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) {
        printf("the node you want to pop is not in the list\n"); 
        return;
    }
    // Remove the node from the list
    prev->next = curr->next;
    node2Pop->next = NULL; 
    if (prev->next == NULL) {
        // If the popped node was the tail, update the tail pointer
        linkedList->tail = prev;
    }
}

void insertAtLinkedListTail(linkedList_t* linkedList, node_t* newNode){
    assert(linkedList != NULL);
    assert(newNode != NULL); 
    newNode->next = NULL; 
    if(linkedList->tail == NULL){
        linkedList->head = linkedList->tail = newNode; 
    }
    else{
        linkedList->tail->next = newNode; 
        linkedList->tail = newNode; 
    }
}

int countLinkedListLength_general(linkedList_t* linkedList){
    assert(linkedList != NULL); 
    if(linkedList->head == NULL){
        return 0; 
    }
    int count = 0; 
    node_t* currNode = linkedList->head; 
    while(currNode != NULL){
        count++; 
        currNode = currNode->next; 
    }
    return count; 
}

int countLinkedListLength(linkedList_t* linkedList, unsigned int time){
    assert(linkedList != NULL); 
    if(linkedList->head == NULL){
        return 0; 
    }
    int count = 0; 
    node_t* currNode = linkedList->head; 
    while(currNode != NULL){
        if(((process_t*)currNode->data)->arriveTime < time){
            count ++; 
        }
        currNode = currNode->next; 
    }
    return count; 
}

void insertAtIndex(linkedList_t* list, node_t* newNode, int index) {
    // If the list is empty, insert the new node as the head
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
        return;
    }

    // If the index is 0, insert the new node at the head
    if (index == 0) {
        newNode->next = list->head;
        list->head = newNode;
        return;
    }

    // Traverse the list to find the node at the specified index
    node_t* current = list->head;
    int currentIndex = 0;
    while (current != NULL && currentIndex < index - 1) {
        current = current->next;
        currentIndex++;
    }

    // If the index is out of range, insert the new node at the tail
    if (current == NULL) {
        printf("index our of range, insert at the end"); 
        list->tail->next = newNode;
        list->tail = newNode;
        return;
    }

    // Insert the new node after the node at the specified index
    newNode->next = current->next;
    current->next = newNode;

    // If the new node is inserted at the tail, update the tail pointer
    if (newNode->next == NULL) {
        list->tail = newNode;
    }
}


void freeNode_process(node_t* node){
    if(node == NULL){
        return; 
    }
    free(((process_t*)node->data)); 
    free(node); 
    node = NULL; 
}

void freeNode_memory(node_t* node){
    if(node == NULL){
        return; 
    }
    free(((memoryChunk_t*)node->data)); 
    free(node); 
    node = NULL; 
}

void freeLinkedList_process(linkedList_t* linkedList){
    node_t *curr, *prev;
	assert(linkedList != NULL);
	curr = linkedList->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		freeNode_process(prev);
	}
	free(linkedList);
    linkedList = NULL; 
}

void freeLinkedList_memory(linkedList_t* linkedList){
    node_t *curr, *prev;
	assert(linkedList != NULL);
	curr = linkedList->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		freeNode_memory(prev);
	}
	free(linkedList);
    linkedList = NULL;
}

void printLinkedList_process(linkedList_t* linkedList){
    if(linkedList->head == NULL){
        printf("This process linked list is null"); 
    }
    node_t* currNode; 
    currNode = linkedList->head; 
    while(currNode){
        printf("%s ", ((process_t*)currNode->data)->name); 
        currNode = currNode->next; 
    }
    printf("\n"); 
}

void printLinkedList_memory(linkedList_t* linkedList){
    if(linkedList->head == NULL){
        printf("This memory linked list is NULL"); 
    }
    node_t* currNode = linkedList->head; 
    while(currNode){
        printf("c-start-%d-size-%d", ((memoryChunk_t*)currNode->data)->start, ((memoryChunk_t*)currNode->data)->size); 
        currNode = currNode->next; 
    }
    printf("\n"); 
}

int search4BestFitMemoryChunk(linkedList_t* linkedList, int requiredSize, int* start){
    //if empty, insert at front
    if(linkedList->head == NULL){
        *start = 0; 
        return 0; 
    }
    int bestFitLocation = -1; 
    int currLocation = 0; 
    int smallestSize = MAX_MEMORY + 1; 
    node_t* prev = NULL;
    node_t* curr = linkedList->head;
    while (curr != NULL || prev != NULL) {
        int prevEnd; 
        int currStart; 
        if(prev == NULL){
            prevEnd = 0; 
        }
        else{
            prevEnd = ((memoryChunk_t*)prev->data)->start + ((memoryChunk_t*)prev->data)->size; 
        }
        if(curr == NULL){
            currStart = MAX_MEMORY; 
        }
        else{
            currStart = ((memoryChunk_t*)curr->data)->start; 
        }
        //printf("%d, %d\n", prevEnd, currStart); 
        int currSize = currStart - prevEnd; 
        if(currSize >= requiredSize && currSize < smallestSize){ 
            smallestSize = currSize; 
            bestFitLocation = currLocation; 
            *start = prevEnd; 
        }
        if(curr == NULL){
            break; 
        }
        prev = curr;
        curr = curr->next;
        currLocation++; 
    }
    return bestFitLocation; 
}

void removeMemoryFromLinkedList(linkedList_t* linkedList, int start){
    assert(linkedList != NULL && 0<=start && start <MAX_MEMORY); 
    node_t* curr = linkedList->head; 
    while(curr != NULL){
        if(((memoryChunk_t*)(curr->data))->start == start){
            popFromLinkedList(linkedList, curr); 
            freeNode_memory(curr); 
            return; 
        }
        curr = curr->next; 
    }
    printf("the memory chunk at this address doesn't exist"); 
}


stats_t* createStats(){
    stats_t* stats = (stats_t*)malloc(sizeof(*stats)); 
    stats->numFinishedProcess = 0; 
    stats->maxOverHead = -1.0f; 
    stats->sumOverHead = 0.0f; 
    stats->sumTurnAround = 0; 
    return stats; 
}
