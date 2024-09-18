# 1\. Assessment

Passed

# 2\. General

* Generally well done, however, no Min-Vertex-Cover algorithm was implemented.

# 3\. Report

## Abstract & Introduction

* Very nice introduction.

## Algorithm & Implementation

* I don't understand the update rule for E in the pseudocode for Alg. 1.
* You only implemented the decision version of the Vertex-Cover problem, the goal was to implement an algorithm for the Minimum-Vertex-Cover problem (s. t. exact solutions are found). As you gradually increased k it is kind of like you search for a small cover, however, it might have been better to explore the vertex covers with a min-priority queue with the key being the size of a given vertex cover and finding methods to restrict the growth of the BST (e.g. checking for tree properties at a given step).
* Your implementation of the 2-Approximation algorithm. is fairly inefficient, it would be better to use a while loop and remove the edges from the graph.
* Careful with L.16 bst_vc -> the loop may run forever if there are no more edges.
* A more straightforward way in the bst algorithm would be to modify the graph with respect to the partial cover and to modify it back after the recursive call.

## Experimental Evaluation

* It is good that you at least gave some sample graphs you could run your BST algorithm on.
* A graph with x corresponding to the instance would have been nice as well.
* Further a graph for the resulting sizes of the covers would have been good.

## Conclusion

* The conclusion is very short, you could have discussed further routes for improvement.

# 4\. Code & Test Cases

* The project structure is well organized. 
* The test cases are alright, although they should readily have the test graphs as input.

Yannick Bosch