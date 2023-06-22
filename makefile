CC = gcc
CFLAGS = -Wall

allocate: allocate.c readIn.c dataStructure.c schedule.c printResult.c controlRealProcess.c
	$(CC) $(CFLAGS) -o allocate allocate.c readIn.c dataStructure.c schedule.c controlRealProcess.c printResult.c -lm

clean:
	rm -f allocate *o