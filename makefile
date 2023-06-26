CC = gcc
CFLAGS = -Wall

all: allocate process
allocate: allocate.c readIn.c dataStructure.c schedule.c printResult.c controlRealProcess.c
	$(CC) $(CFLAGS) -o allocate allocate.c readIn.c dataStructure.c schedule.c controlRealProcess.c printResult.c -lm

process: process.c 
	$(CC) $(CFLAGS) -o process process.c

clean:
	rm -f allocate *o process