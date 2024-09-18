#!/bin/bash

set -e

# Specify the path to the executable and input file
EXECUTABLE="../../ex3_vc_validate/bin/vc_validate"
# INPUT_FILE="input/vc-exact_099.gr"

# Loop from 0 to 200 and invoke the command for each number
for ((i=1; i<=200; i++)); do

    padded_i=$(printf "%03d" $i)

    OUTPUT_FILE="${padded_i}.vc"
    COMMAND="${EXECUTABLE} -g input/vc-exact_${padded_i}.gr -vc out/bst/${OUTPUT_FILE}"
    COMMAND="${EXECUTABLE} -g input/test/test_${i}.gr -vc out/bst/${OUTPUT_FILE}"
    echo "Running: ${COMMAND}"
    ${COMMAND}
done
