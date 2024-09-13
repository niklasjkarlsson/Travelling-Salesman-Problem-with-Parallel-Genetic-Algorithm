#!/bin/bash

N=20
P=10
args="../run/TSP_data100.dat 500 10000 1 20"
rm -f avgs.dat

# Loop over the number of processes
for p in $(seq 1 $P); do
    sum=0
    # Loop over the number of iterations
    for i in $(seq 1 $N); do
        # Get the time value from the command's last line
        time=$(mpirun -np $p g $args | awk 'END{print $NF}')
        # Accumulate the time values
        sum=$(echo "$sum + $time" | bc)
    done
    # Compute the average and append to avgs.dat
    avg=$(echo "scale=2; $sum / $N" | bc)
    echo $avg >> avgs.dat
done
		  
