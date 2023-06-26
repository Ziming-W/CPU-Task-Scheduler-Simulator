
commands=(
    # Task 1 - SJF (1 mark)
    ## With double the processes
    "./allocate -f hidden/task1/shortest-first.txt -s SJF -m infinite -q 3| diff - hidden/task1/shortest-first.out"
    "./allocate -f hidden/task1/even-more-processes.txt -s SJF -m infinite -q 3| diff - hidden/task1/even-more-processes.out"
    ## With ties in arrival time, i.e. need to sort by name
    "./allocate -f hidden/task1/order.txt -s SJF -m infinite -q 2| diff - hidden/task1/order.out"
    
    # Task 2 - RR (1.5 marks)
    ## More processes
    "./allocate -f hidden/task2/more-processes-1.txt -s RR -m infinite -q 1| diff - hidden/task2/more-processes-1.out"
    "./allocate -f hidden/task2/more-processes-2.txt -s RR -m infinite -q 2| diff - hidden/task2/more-processes-2.out"
    ## 20 processes
    "./allocate -f hidden/task2/20-processes.txt -s RR -m infinite -q 2| diff - hidden/task2/20-processes-q2.out"
    "./allocate -f hidden/task2/20-processes.txt -s RR -m infinite -q 3| diff - hidden/task2/20-processes-q3.out"
    
    # Task 3 - best-fit (1.5 marks)
    ## Gaps edge SJF |E1|S1|E2|
    "./allocate -f hidden/task3/gaps-edge.txt -m best-fit -q 1 -s SJF| diff - hidden/task3/gaps-edge.out"
    ## Gaps surround |E1|B1|B2|B3|E2|
    "./allocate -f hidden/task3/gaps-surround.txt -m best-fit -q 3 -s RR| diff - hidden/task3/gaps-surround.out"
    ## last-in SJF
    "./allocate -f hidden/task3/last-in.txt -m best-fit -q 3 -s SJF| diff - hidden/task3/last-in-spf.out"
    
    # Task 4 - Real process (1.5 marks)
    ## Normal tests
    "./allocate -f hidden/task4/more-processes-4.txt -s SJF -m infinite -q 2| diff - hidden/task4/more-processes-4.out"
    "./allocate -f hidden/task2/20-processes.txt -s RR -m infinite -q 3| diff - hidden/task2/20-processes-q3.out"
    ## > 1000 process RR best-fit -- The ultimate
    "./allocate -s RR -m best-fit -f hidden/task4/🙂dir1_long-long()_0*./comp30023/../comp30023/600.txt -q 3| diff - hidden/task4/🙂dir1_long-long()_0*./comp30023/600.out"
    "./allocate -s RR -m best-fit -f hidden/task4/🙂dir1_long-long()_0*./comp30023/../comp30023/1200.txt -q 3| diff - hidden/task4/🙂dir1_long-long()_0*./comp30023/1200.out"
    
    # Task 5 - Perf (1 mark, 0.25 each taken from previous tests)
    "./allocate -f hidden/task5/more-processes-5.txt -s SJF -m infinite -q 2| diff - hidden/task5/more-processes-5.out"
    "./allocate -f hidden/task1/shortest-first.txt -s SJF -m infinite -q 3| diff - hidden/task1/shortest-first.out"
    "./allocate -f hidden/task2/more-processes-2.txt -s RR -m infinite -q 2| diff - hidden/task2/more-processes-2.out"
    "./allocate -f hidden/task3/last-in.txt -m best-fit -q 3 -s RR| diff - hidden/task3/last-in-rr.out"
    
    # # Cases from Ed
    # "-f /hidden/ed/189.txt -s SJF -m infinite -q 3| diff - hidden/ed/189.out"

    # "-f hidden/ed/generated-tests/data0.txt -s SJF -m best-fit -q 1| diff - hidden/ed/generated-tests/output0.txt"
    # "-f hidden/ed/generated-tests/data1.txt -s SJF -m best-fit -q 3| diff - hidden/ed/generated-tests/output1.txt"
    # "-f hidden/ed/generated-tests/data2.txt -s RR -m infinite -q 2| diff - hidden/ed/generated-tests/output2.txt"
    # "-f hidden/ed/generated-tests/data3.txt -s SJF -m infinite -q 3| diff - hidden/ed/generated-tests/output3.txt"
    # "-f hidden/ed/generated-tests/data4.txt -s RR -m best-fit -q 3| diff - hidden/ed/generated-tests/output4.txt"
    # "-f hidden/ed/generated-tests/data5.txt -s SJF -m best-fit -q 2| diff - hidden/ed/generated-tests/output5.txt"
    # "-f hidden/ed/generated-tests/data6.txt -s RR -m best-fit -q 1| diff - hidden/ed/generated-tests/output6.txt"
    # "-f hidden/ed/generated-tests/data7.txt -s RR -m infinite -q 3| diff - hidden/ed/generated-tests/output7.txt"
    # "-f hidden/ed/generated-tests/data8.txt -s RR -m infinite -q 3| diff - hidden/ed/generated-tests/output8.txt"
    # "-f hidden/ed/generated-tests/data9.txt -s SJF -m infinite -q 1| diff - hidden/ed/generated-tests/output9.txt"
    
    # # h,echo,,Extra (0 marks)
    # "-f hidden/extra/skip.txt -s SJF -m infinite -q 1| diff - hidden/extra/skip.out"
    # "-f hidden/extra/skip.txt -s SJF -m infinite -q 1| diff - hidden/extra/skip.out"
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
    echo "----------------------"
done