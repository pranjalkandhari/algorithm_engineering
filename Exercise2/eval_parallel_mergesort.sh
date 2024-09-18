#!/bin/bash


# Specify the range of ram_share values you want to test
# 10%, 50%, 100%, 1000%
n=("100" "1000" "10000" "100000" "1000000" "10000000" "100000000" "1000000000")
# n=("1000000")

#n_cores=("1" "2" "3" "4" "5" "6" "7" "8" "9" "10")
n_cores=("4")

# Specify the number of runs
n_runs=1

for nc in "${n_cores[@]}"; do
  # Loop over ram_share values and call the executable
  for nn in "${n[@]}"; do
    # Construct the command to call the executable
    command="./build/OMPMergeSort $nn $n_runs $nc"

    # Print the command to the console (optional)
    echo "Running: $command"

    # Run the command and capture the average sorting duration
    average_duration=$( $command | grep "Average sorting duration" | awk '{print $NF}' )

    # Print the average duration to the console (optional)
    echo "Average sorting duration for $nn numbers with $nc cores: $average_duration"

    # Save the average duration to a file
    echo "$average_duration" > "result_${ram_share}.txt"

    # Print a separator line for better readability (optional)
    echo "------------------------"
  done
done



echo "All tests completed."
