# Build project

## Prerequisitories

- To build the project, make sure to have `cmake` and the Boost c++ library availabe
- To run the project, make sure to have the `timeout` command available
- To test the project, make sure to setup the project `https://github.com/hmarkus/vc_validate` as described in the repository

## Build

- `mkdir build`
- `mkdir out`
- `mkdir out/2apx && mkdir out/bst`
- `cmake -DOPTIMIZED_RELEASE=ON -S . -B build`
- `cmake --build build`


# Run

## Vertex Cover Computation

- `run_2apx.sh` to run 2-APX algorithm over whole dataset
- `run_bst.sh` to run BSt algorithm over whole dataset

- `build/src/BST --help` for concrete usage info
- `build/src/2APX --help` for concrete usage info

# Vertex Cover Computation
- use `test_2apx.sh` resp. `test_bst.sh` to use the above referenced testing utility to validate the calculated vertex cover
  - in the script, make sure to set the path to `EXECUTABLE` correct, in case `vc_validate` is checked out at a seperate location
