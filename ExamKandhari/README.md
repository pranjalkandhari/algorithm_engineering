# Compilation command:
g++ feedback_vertex_set.cpp

# Execution command:
Windows: a.exe
Mac/Linux: ./a.out

# Files:
Executing file: feedback_vertex_set.cpp
Algorithms are written is: algorithms.h
Utility/Common functions are written in utility.cpp

Other Python and Jupyter notebooks are data curating, cleaning and plot generation scripts.

# Data set:
Prepared graphs (using refactor_graph.py) are saved in the folder: input_data
Raw graph data is in the folder: PACE

# Output:
2 ouput files are placed in the results folder as samples: .txt files.
The results folder also contains the plots used in the report.

# Configuration:
K_LIMIT_EXP (in feedback_vertex_set.cpp): Tells the max k for which the exponential algorithms should run.
RUN_EXP_ALGORITHMS (in feedback_vertex_set.cpp): If false: the exponential algorithms do not work.
