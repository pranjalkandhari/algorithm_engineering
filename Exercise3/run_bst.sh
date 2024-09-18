#!/bin/bash

# Specify the path to the executable and input file
EXECUTABLE="build/src/BST"

TIMEOUT_LIMIT=600

echo "idx, k, time, vc_size" > "out/report_bst.csv"

# Loop from 0 to 200 and invoke the command for each number
for ((i=1; i<=200; i++)); do

    padded_i=$(printf "%03d" $i)

    k_values=("5" "10" "20" "50" "100" "200" "400" "800" "1600" "2400" "3600")
    # k_values=("5" "10" "15" "20" "25" "30" "35" "40" "45" "50")

    for k in "${k_values[@]}"; do
        OUTPUT_FILE="${padded_i}.vc"
        COMMAND="timeout ${TIMEOUT_LIMIT} ${EXECUTABLE} -gr input/vc-exact_${padded_i}.gr -vc out/bst/${OUTPUT_FILE} -k ${k}"
        # COMMAND="timeout ${TIMEOUT_LIMIT} ${EXECUTABLE} -gr input/test/test_${i}.gr -vc out/bst/${OUTPUT_FILE} -k ${k}"

        echo "Running: ${COMMAND}"
        OUT=$($COMMAND)
        OUT_STATUS=$(echo $?)
        NO_VERTEX_COVER_FOUND=$(echo "$OUT" | grep "No vertex cover found" | wc -l)

        if [ $NO_VERTEX_COVER_FOUND -eq 1 ]; then
          echo "No vertex cover found";
        elif [ "$OUT_STATUS" == "124" ]; then
          echo "timelimit reached";
          TIMEOUT_LIMIT_MS=$(( $TIMEOUT_LIMIT*1000000 ))
          echo "$padded_i, $k, $TIMEOUT_LIMIT_MS, 0" >> "out/report_bst.csv"
          break;
        else
          TIME=$(echo "$OUT" | grep -oE 'took ([0-9]+)' | awk '{print $2}')
          VC_SIZE=$(echo "$OUT" | grep -oE 'Length: ([0-9]+)' | awk '{print $2}')
          echo "$OUT";
          echo "$padded_i, $k, $TIME, $VC_SIZE" >> "out/report_bst.csv"
          break;
        fi
    done




#    echo $($OUT | grep )
done
