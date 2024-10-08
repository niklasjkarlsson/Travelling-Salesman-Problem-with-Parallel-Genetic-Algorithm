
This source code directory contains the parallel genetic algorithm of
travelling salesman problem, and python scripts for generating and plotting data
--------------------------------------------------------------------------------


Compiling
--------------------------------------------------------------------------------

The C++ genetic algorithm should be 
compiled with command:

  mpic++ -O3 -o GA_TSP_parallel GA_TSP_parallel.cpp


Running the programs
--------------------------------------------------------------------------------

The genetic algorithm can be run with a command

  mpirun -np <N> GA_TSP_parallel <filename>
      <population_size> <generations> <mutation_interval>
      <migration_interval>

The arguments are:

  <N>                  number of processes for MPI.
    
  <filename>           input file from ../run directory named
                       with format TSP_data().dat
			 
  <generations>        Number of crossover generations.
    
  <population_size>    Total amount of chromosomes.
    
  <migration_interval> Interval of crossovers for migration
                       between processes.

  <mutation_interval>  interval of crossovers at which mutation
                       occurs.

------------------------------------------------------------

plotTSroute.py is a script for plotting the route from
outputfile of the genetic algorithm run. Run with:

  python3 plotTSroute.py <filename>

the input file can be found from ../run directory in
format best_chromosome().dat

------------------------------------------------------------

generate_data.py is a script to generate data for TSP and
takes no arguments. Outputs files with 10, 50, 100 and 200
cities. Run as:

  python3 generate_data.py



-------------------------------------------------------
test_run.sh is a shellscript for measuring average
wall clock times for different processes

