# Compile algorithmns
- `mkdir build`
- `cd build`
- `cmake -DOPTIMIZED_RELEASE=ON ..`
- `cmake --build .`

# Running commands for quick sort:
g++ -fopenmp quick_sort.cpp

Arguments in the run command:  <Number of processors> <Number of elements> <True/False for parallel sorting>
For, p=8, n=1000 and paralllelization = true:

Windows:
a.exe 8 1000 true

Mac:
./a.out 8 1000 true