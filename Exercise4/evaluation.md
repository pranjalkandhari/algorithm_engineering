# 1\. Assessment

Passed

# 2\. General

* The report is well done. However, the running time analysis for the nearest neighbor heuristic is flawed.

# 3\. Report

## Abstract & Introduction

* Nice introduction to the problem and what you did.

## Algorithm and Implementation

* The nearest neighbor heuristic has a naive running time of n^2 (finding the node that has not been visited and is closest to the current node takes O(n)).
* For 2-Opt you shouldn't take a distance matrix but do the distance computations on the fly. However, if your system could handle the 
memory used that is fine of course. Further you should only swap after the loops finish (or left the loops, if an improvement is found), swapping while keeping the iteration could have unwanted side effects.

## Experimental Evaluation

* In the tables you should have also computed the ratios between your results and the lower-bounds.
* The 2-OPT NN Time should include the Nearest neighbor heuristic computation.
* Your figures are all over the place. Consider using `subfigure` to group your plots and `[htb!]` to position them. 

## Conclusion

* The 2-opt algorithm should not be faster than the nearest neighbor algorithm.
* If you stop the execution of the 2-opt algorithm at a certain point, you cannot make any valuable comparisons to the nearest neighbor heuristic running time.
* You should have recorded the time limit used by your 2-opt algorithm.

# 4\. Code & Test Cases

* Please use `±std::numeric_limits<double>::infinity()` in your code for min_distance/max_distance computations.

* Yannick Bosch
