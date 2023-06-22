#!/bin/bash

# Ziming's own script for test all cases at once
# Define an array of commands
commands=(
    "./allocate -f cases/task1/simple.txt -s SJF -m infinite -q 1 | diff - cases/task1/simple-sjf.out"
    "./allocate -f cases/task1/more-processes.txt -s SJF -m infinite -q 3 | diff - cases/task1/more-processes.out"
    "./allocate -f cases/task2/simple.txt -s RR -m infinite -q 3 | diff - cases/task2/simple-rr.out"
    "./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 1 | diff - cases/task2/two-processes-1.out"
    "./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 3 | diff - cases/task2/two-processes-3.out"
    "./allocate -f cases/task3/simple.txt -s SJF -m best-fit -q 3 | diff - cases/task3/simple-bestfit.out"
    "./allocate -f cases/task3/non-fit.txt -s SJF -m best-fit -q 3 | diff - cases/task3/non-fit-sjf.out"
    "./allocate -f cases/task3/non-fit.txt -s RR -m best-fit -q 3 | diff - cases/task3/non-fit-rr.out"
    "./allocate -f cases/task4/spec.txt -s SJF -m infinite -q 3 | diff - cases/task4/spec.out"
)

# Loop over the commands and run each one
for command in "${commands[@]}"; do
    result=$(eval "$command")
    if [ -z "$result" ]; then
        echo "Command succeeded: $command"
    else
        echo "Command failed: $command"
        echo "$result"
    fi
done