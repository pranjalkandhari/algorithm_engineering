# 1\. Assessment

(STILL) passed

# 2\. General

* The code is pretty unreadable as variable names often consist only of one character. If that doesn't change in the next 
assignments, the grading will be harder. 
* The code runs extremely slow.

# 3\. Report
* Okay. The evaluation and conclusion are thin.

## Abstract & Introduction

* Nicely introduced the necessity for parallel sorting algorithms. 
* You don't have to write down the PRAM Model.
* The pseudocode for quickSort uses additional arrays, that is not necessary, as it can work in-place.

## Algorithm & Implementation

* 3.1 missing O.
* 3.2 in the runtime analysis it seems like you mixed up work and depth.
## Experimental Evaluation

* It is very unorthodox to compare running times of algorithms that were run on different machines.
* The plots are not very meaningful and hard to read. A plot comparing the running-times for all for algorithms would 
have been nice.
* Parallel MergeSort and parallel QuickSort should get faster with more processors.
* The analysis of Quicksort and Parallel QuickSort is badly described, the plots are not explained.
* No comparison to the classical algorithms is done.
## Conclusion

* The conclusion is very short and doesn't really explain why the parallelized versions are slower.

# 4\. Code & Test Cases

* The code does not directly compile, as in the cmake file, there is no sign of including openMP.
* Provide easy to execute compile commands like 'g++ ...' for EVERY target, rather than providing a cmake build.
* Provide execution samples for EVERY target 'quicksort 8 1000 true'.
* Please name your executables something else like 'a.out'
* parallel for is not the right choice for nested parallelization, rather it is recommended to use taskloop/taskgroup and tasks.
* strcmpi is microsoft specific, please only use functions from the std library or provide libraries you use.
* In both algorithms no outer parallelization was performed, which is critical for good performance.

* Yannick Bosch