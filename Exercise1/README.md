# Mergesort

### Compile Mergesort
g++ -std=c++17 lib/*.cpp mergesort.cpp -o mergesort

### Run Mergesort
USAGE: mergesort [ram_share] [n_runs] [test]

ram_share: Size of file in terms of RAM size (0.1: 10% size, 10: 1000% size).
n_run: Number of randomly generated test cases: Averaged to find the execution time.
test: 1: to compare the output with the original input. 0: not to compare.

eg. `./mergesort 0.1 2 1`


# EM Mergesort
### Compile EM Mergesort
g++ -std=c++17 lib/*.cpp em_mergesort.cpp -o em_mergesort

# Run EM Mergesort
USAGE: em_mergesort [blocksize] [ram_share] [n_runs] [test]

blocksize: i/o size in bytes (B).

eg. `./em_mergesort 2048 0.001 2 0`