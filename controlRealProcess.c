/*For task 4-control real process*/

/*Notice that, there are some redundancy in this code, for example, write->kill->read->verify can be pipelined*/
/*But since most instructions are just a single line, e.g. read(process->pipes_out[0], &byte, sizeof(byte)) != sizeof(byte)*/
/*For a clear presentation and readablity purpose I didn't put them into functions*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <signal.h>

#include "controlRealProcess.h"
#include "printResult.h"

int createRealProcess(process_t* process, unsigned int time) {
    //if this process haven't been created
    if(process->pid != -1){
        perror("Create:trying to create a process that has been created already"); 
    }
    pid_t pid;
    if (pipe(process->pipes_in) < 0 || pipe(process->pipes_out) < 0) {
        perror("Create:pipe");
        return -1;
    }
    if ((pid = fork()) < 0) {
        perror("Create:fork");
        return -1;
    } else if (pid == 0) {

        // Redirect standard input and output to pipes
        if (dup2(process->pipes_in[0], STDIN_FILENO) == -1 || dup2(process->pipes_out[1], STDOUT_FILENO) == -1) {
            perror("Create:dup2");
            exit(EXIT_FAILURE);
        }
        // Execute the process
        //char* args[] = {"./process", "-v", process->name, NULL};
        char* args[] = {"./process", process->name, NULL};
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // Write the 32-bit value to the process's standard input
        unsigned int value = htonl(time);
        if (write(process->pipes_in[1], &value, sizeof(value)) != sizeof(value)) {
            perror("Create:write:");
            return -1;
        }
        // read 1 byte from the process's standard output
        unsigned char byte;
        if (read(process->pipes_out[0], &byte, sizeof(byte)) != sizeof(byte)) {
            perror("Create:read");
            return -1;
        }
        // Verify that it matches the least significant byte of the value
        if (byte != ((time >> 0) & 0xFF)) {
            fprintf(stderr, "createProcess: Verification failed: expected %02X, got %02X\n", (time >> 0) & 0xFF, byte);
            return -1;
        }

        process->pid = pid;
    }
    return 0;
}

int suspendRealProcess(process_t* process, unsigned int time) {
    //if this process haven't been created
    if(process->pid == -1){
        perror("Suspend:trying to suspend a process which hasn't been created");
        return -1;  
    }
    // write the 32-bit value to the process's standard input
    unsigned int value = htonl(time);
    if (write(process->pipes_in[1], &value, sizeof(value)) != sizeof(value)) {
        perror("Suspend:write:");
        return -1;
    }

    // send the SIGTSTP signal to process
    if (kill(process->pid, SIGTSTP) == -1) {
        perror("Suspend:SIGTSTP");
        return -1;
    }   

    int wstatus;   // Wait for the process to enter a stopped state

    pid_t w;
    do {
        w = waitpid(process->pid, &wstatus, WUNTRACED);
        if (w == -1) {
            perror("Suspend: waitpid");
            return -1;
        }
    } while (!WIFSTOPPED(wstatus));
    return 0;
}

int continueRealProcess(process_t* process, unsigned int time){
    //if this process haven't been created
    if(process->pid == -1){
        perror("Continue:trying to continue a process which hasn't been created"); 
    }
    // Send simulation time to process
    unsigned int value = htonl(time);
    if (write(process->pipes_in[1], &value, sizeof(value)) != sizeof(value)) {
        perror("continue: write");
        return -1;
    }

    // send SIGCONT signal to process
    if (kill(process->pid, SIGCONT) == -1) {
        perror("continue: SIGCONT");
        return -1;
    }

    // Read 1 byte from the standard output of process and verify
    unsigned char byte;
    if (read(process->pipes_out[0], &byte, sizeof(byte)) != sizeof(byte)) {
        perror("Continue:read");
        return -1;
    }
    if (byte != ((time >> 0) & 0xFF)) {
        fprintf(stderr, "continueProcess:Verification failed: expected %02X, got %02X\n", (time >> 0) & 0xFF, byte);
        return -1;
    }

    return 0;
}

int terminateRealProcess(process_t* process, unsigned int time) {
    // Write the 32-bit value to the process's standard input
    unsigned int value = htonl(time);
    if (write(process->pipes_in[1], &value, sizeof(value)) != sizeof(value)) {
        perror("Termination:write - ");
        return -1;
    }

    // Send SIGTERM signal to process
    if (kill(process->pid, SIGTERM) < 0) {
        perror("Termination:kill");
        return -1;
    }

    // read from the process's standard output
    char output[65];
    if (read(process->pipes_out[0], output, sizeof(output) - 1) < 0) {
        perror("Termination:read");
        return -1;
    }

    // null-terminate the output and print it out
    output[64] = '\0';
    printFinishHash(time, process->name, output); 
    process->pid = -1; 

    return 0;
}
