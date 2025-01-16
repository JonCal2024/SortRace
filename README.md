# Sort Race (CECS325)
Radix sort race against the generic system sort. Coded with C++ in a Linux environment.



### generate.cpp 
This program runs from the command line and takes in 3 additional arguments: count, min and max. It then generates a count amount of numbers between the range of min and max and outputs them line by line to file numbers.dat. 

### mysort.cpp
This program takes in the names of an input and output file from the command line, takes all of the numbers from the input file and sorts them in groups using threaded sorts, merges the whole array, and outputs the sorted array to stdout. 

### mysort.out
Sorted number output file for mysort. Compared to systemsort.out with diff to test mysort's accuracy.

### numbers.dat
Output file generated from generate.cpp, acts as input file for mysort.

### sortrace.log
Record for the statistics and outcome of sorting race.

### sortrace.sh
Script file for running generate and mysort, then storing outcome in sortrace.log.

### systemsort.out
Sorted number output file for system sort. Compared to mysort.out to test mysort's accuracy.
