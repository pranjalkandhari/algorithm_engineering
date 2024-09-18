# 1\. Assessment

Passed

# 2\. General

* Very nice project, good report and results. The code is structured, good job!

# 3\. Report

## Abstract & Introduction

* Nice and concise Introduction of the problem. The introduction however, directly repeats the Abstract. 
## Algorithm & Implementation

* Some pseudocode for your algorithm would have been good, as well as some more theoretical discussion about the running 
times of your implementation. Otherwise you clearly laid out the foundations of both algorithms and didn't stretch it out to long, very nice.
* During sort_partitions you should have sorted blocks that were of the size of the entire RAM, however only blocksize chunks are sorted.
* You should give the sorted output a concise name, as it is not clear in which file the sorted output is to be.
* There is a lot of duplicated code in merge_two, please make sure to next time put that into its own function.
## Experimental Evaluation

* Very nice experimental evaluation. The Data is clearly presented and the results are as expected.

## Conclusion

* The conclusion is well written and the results are correct. The reason as for why EM-Merge-sort is slower is also discussed - Good Job!
# 4\. Code & Test Cases

* Nicely implemented the algorithms. Instead of hardcoding a getSystemRam function, you also could have provided the RAM size as an argument, for more control, 
* and such that one doesn't need to change your code (Also the number doesn't correspond to the comment 2GB instead of 16GB). The test case is well described. The only downside is that due to using binary 
files, we can't really check if the sequence is sorted.
* Please be sure to fully implement your algorithms in its own function and not the main function. This will increase readability and reusability.

YOUR:NAME