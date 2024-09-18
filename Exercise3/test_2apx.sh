#!/bin/bash

set -e

# Specify the path to the executable and input file
EXECUTABLE="../../ex3_vc_validate/bin/vc_validate"
# INPUT_FILE="input/vc-exact_099.gr"

# Loop from 0 to 200 and invoke the command for each number
for ((i=1; i<=200; i++)); do

    padded_i=$(printf "%03d" $i)

    OUTPUT_FILE="${padded_i}.vc"
    COMMAND="${EXECUTABLE} -g input/vc-exact_${padded_i}.gr -vc out/2apx/${OUTPUT_FILE}"
    # COMMAND="./build/src/2APX -gr input/vc-exact_001.gr -vc out/001.vc"
    # COMMAND="./build/src/2APX -gr input/vc-exact_001.gr"
    echo "Running: ${COMMAND}"
    ${COMMAND}
done
