#!/bin/bash

# Specify the path to the executable and input file
EXECUTABLE="build/src/2APX"
# INPUT_FILE="input/vc-exact_099.gr"

echo "idx, time, vc_size" > "out/report_2apx.csv"

# Loop from 0 to 200 and invoke the command for each number
for ((i=1; i<=200; i++)); do

    padded_i=$(printf "%03d" $i)

    OUTPUT_FILE="${padded_i}.vc"
    COMMAND="${EXECUTABLE} -gr input/vc-exact_${padded_i}.gr -vc out/2apx/${OUTPUT_FILE}"
#    COMMAND="${EXECUTABLE} -gr input/test/test_${i}.gr -vc out/2apx/${OUTPUT_FILE}"

    echo "Running: ${COMMAND}"
    OUT=$($COMMAND)
    echo "$OUT"
    TIME=$(echo $OUT | grep -oE 'took ([0-9]+)' | awk '{print $2}')
    VC_SIZE=$(echo $OUT | grep -oE 'Length: ([0-9]+)' | awk '{print $2}')
    echo "$padded_i, $TIME, $VC_SIZE" >> "out/report_2apx.csv"
#    echo $($OUT | grep )
done
