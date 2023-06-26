## Purpose
A CPU task scheduler simulator. 
Project work for COMP30023(Computer System) at University of Melbourne.  
`Sheduling option`: Round Robin | Shortest Job First  
`Memory allocation option`: infinite memory | best-first

## File Purpose
| File    | Description |
| -------- | ------- |
|readIn.c/readIn.h | Parse command line argument, read in processes input from text file    |
| dataStructure.c/dataStructure.h| data structure helpers |
|schedule.c/schedule.h| Core task sheduling logic|
|printResult.c/printResult.h|helpers to print transcript to stdout|
|process.c|Authorized by teaching team, mock processes|
|controlRealProcess.c/controlRealProcess.h|collaborate with process.c, perfrom real multi-processing|
|allocate.c|main entry point|



## Compile

    make

## How to run a single test case

    allocate -f <filename> -s (SJF | RR) -m (infinite | best-fit) -q (1 | 2 | 3)

## example
    ./allocate -f cases/task1/simple.txt -s SJF -m infinite -q 1 | diff - cases/task1/simple-sjf.out


Notice: q is the quantum

## Test script to run all tests
    bash testVisible.sh