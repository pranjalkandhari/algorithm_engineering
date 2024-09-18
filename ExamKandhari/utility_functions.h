#include <iostream>
#include <vector>
#include <map>
using namespace std;

// Function to remove/disconnect a vertex in the adjacency matrix
void disconnect_vertex(vector<vector<int> > &adj_mat, int node) {
    int n = adj_mat.size();
    for (int i=0; i<n; i++) {
        adj_mat[i][node] = 0;
        adj_mat[node][i] = 0;
    }
}

// Function to add a vertex in the adj_mat_without_cycle
void add_vertex(vector<vector<int> > adj_mat, vector<vector<int> > &adj_mat_without_cycle, int node, set<int> nodes_in_graph) {
    for (int i: nodes_in_graph) {
        adj_mat_without_cycle[node][i] = adj_mat[node][i];
        adj_mat_without_cycle[i][node] = adj_mat[i][node];
    }
}

// Checks if there is a cycle in the given component of the graph:
bool has_cycle_dfs(std::vector<std::vector<int>>& adj_mat, int start_node, int parent_node, vector<bool> &visited) {
    if (visited[start_node]) {
        return true;
    }
    visited[start_node] = true;

    int n = adj_mat.size();

    for (int i=0; i<n; i++) {
        if (i != parent_node && adj_mat[start_node][i] != 0 && has_cycle_dfs(adj_mat, i, start_node, visited)) {
            return true;
        }
    }
    return false;
}

// Returns true if there are no cycles in the graph. Else: false.
bool no_cycle_detected(std::vector<std::vector<int>>& adj_mat) {
    int n = adj_mat.size();
    vector<bool> visited(n, false);

    for (int start_node=0; start_node<n; start_node++) {
        if (!visited[start_node]) {
            // Calling for all components of the graph:
            if (has_cycle_dfs(adj_mat, start_node, -1, visited)) {
                return false;
            }
        }
    }
    return true;
}

// Function to print the adjacency matrix:
void print_adj_mat(vector<vector<int> > &adj_mat) {
    cout<<"Printing ADJ MAT:"<<endl;

    for (int i=0; i<adj_mat.size(); i++) {
        for (int j=0; j<adj_mat[0].size(); j++) {
            cout<<adj_mat[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"--------------------"<<endl;
}

// clear function: Applies the Short-Cut method to the graph till it is possible to apply.
set<int> clear(vector<vector<int> > &adj_mat) {
    set<int> disconnected_elements_set;
    while (true) {
        vector<int> nodes_to_disconnect;
        int n = adj_mat.size();
        for (int row=0; row<n; row++) {
            vector<int> neighbors;
            for (int col =0; col<n; col++) {
                if (adj_mat[row][col] == 1) {
                    neighbors.push_back(col);
                }
            }
            // Now our neigbours vector is ready.
            // Delete element if:
            // 1) Degree == 1
            // 2) Degree == 2 but the 2 neighbors are not neighbors of each other.
            if (neighbors.size() == 1 || (neighbors.size() == 2 && adj_mat[neighbors[0]][neighbors[1]] == 0)) {
                nodes_to_disconnect.push_back(row);
            }
        }

        if (nodes_to_disconnect.size() == 0) {
            // No shortcut possible: Break the loop.
            return disconnected_elements_set;
        }

        // Deleting elements:
        for (int i=0; i<nodes_to_disconnect.size(); i++) {
            disconnect_vertex(adj_mat, nodes_to_disconnect[i]);
            disconnected_elements_set.insert(nodes_to_disconnect[i]);
        }
    }
    return disconnected_elements_set;
}

// Find the degrees of the requred_elements and returns it is a map.
map<int, int> find_degrees(vector<vector<int> > &adj_mat, vector<int> &requred_elements) {
    int n = adj_mat.size();
    map<int, int> node_degree_map;
    for (int i=0; i<requred_elements.size(); i++) {
        int node = requred_elements[i];
        int degree = 0;
        for (int j=0; j<n; j++) {
            if (adj_mat[node][j] == 1) {
                degree+=1;
            }
        }
        node_degree_map[node] = degree;
    }
    return node_degree_map;
}


// Cite: https://www.geeksforgeeks.org/next-higher-number-with-same-number-of-set-bits/
unsigned int find_next_num_same_setbits(unsigned int x)
{
 
  unsigned int rightOne;
  unsigned int nextHigherOneBit;
  unsigned int rightOnesPattern;
 
  unsigned int next = 0;
 
  if(x)
  {
 
    // right most set bit
    rightOne = x & -(signed)x;
 
    // reset the pattern and set next higher bit
    // left part of x will be here
    nextHigherOneBit = x + rightOne;
 
    // nextHigherOneBit is now part [D] of the above explanation.
 
    // isolate the pattern
    rightOnesPattern = x ^ nextHigherOneBit;
 
    // right adjust pattern
    rightOnesPattern = (rightOnesPattern)/rightOne;
 
    // correction factor
    rightOnesPattern >>= 2;
 
    // rightOnesPattern is now part [A] of the above explanation.
 
    // integrate new pattern (Add [D] and [A])
    next = nextHigherOneBit | rightOnesPattern;
  }
 
  return next;
}
 