# Exam

## build project
- `mkdir build`
- `cd build`
- `cmake -DOPTIMIZED_RELEASE=ON ..` (optional -DOPTIMIZED_RELEASE=OFF for debug)
- `cmake --build .`

## execution
This project contains four scripts in the `scripts` directory. They are used to run the three different algorithms on the PACE graphs. 

The scripts are:
- `run_exact.sh`
- `run_approximation.sh`
- `run_approximation_parallel.sh`
- `run_heuristic.sh`

For each script check inside that the parameters `EXECUTABLE`, `REPORT_FILE` and `OUT_FOLDER` are set correctly on your system.

Make sure that `OUT_FOLDER` exists. 
Thist could be created by running `mkdir -p output/Track1/exact`, `mkdir -p output/Track1/approximation`, `mkdir -p output/Track1/approximation_parallel` and `mkdir -p output/Track1/heuristic`
## dependencies
The project depends on the following preinstalled libraries:
- OpenMP
- Boost

Other dependencies are handled by CMake.

## test
For the heuristic and approximation algorithm, the test cases are provided by executing the respective file.
The resulting steiner tree is validated by checking the following properties:
- The tree is a valid tree
- each terminal is in the tree

As for the exact algorithm, only the size is calculated, this resulting size is compared to the size of the optimal solution.
