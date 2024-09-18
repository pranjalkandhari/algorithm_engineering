# Compiling travelling salesman problem solution:
g++ tsp.cpp

# Executing:

Mac: ./a.out

Windows: ./a.exe

# Execution: requirments already satisfied.
Cloning of the project should work.

## input_data folder:

input.txt have data for all countries in the format (n: total number of points):

$n$

$x_{0}$ $y_{0}$

$x_{1}$ $y_{1}$

$.$

$.$

$.$

$x_{n-1}$ $y_{n-1}$


This format was created using the **file_generator.ipynb** file.


## output_data folder:

Also, an empty folder output_data is made where all the output files will be stored.

### Output file naming format:

SNo_country_a: The output of the nearest neighbor algorithm.

SNo_country_b: The initial path of the 2 opt algorithm.

SNo_country_c: The state of the 2-opt algorithm in the approximate middle of the number of iterations.

SNo_country_d: The final output of the 2-opt algorithm.

SNo_country_e: The final output of the 2-opt algorithm when the nearest neighbor output is given as the initial path for the 2-opt algorithm.

### Output format:

The output contains the path the traveling salesman should take in n lines in each of the format:

from_node to_node distance

distance: euclidean distance between the from_node and to_node.

### Visualizing path:

**visualize_path.R** is used to create visualization of the path using the output data.