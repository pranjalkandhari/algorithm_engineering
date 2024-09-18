#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <thread>
#include <algorithm>
#include <chrono>
#include "utility_functions.h"
using namespace std;


int time_limit = 1;
bool has_timed_out = false;


vector<int> base_case_set;
// Function to tell if a feedback vertex set is possible for a given target size.
// Returns true if the feedback set is possible of the target size and save the set in base_case_set.
bool feedback_set_possible(vector<vector<int> > &adj_mat, vector<int> &elements_to_check, int start, 
                                        vector<int> &feedback_vertex_set, int target_size) {
    if (start >= elements_to_check.size()) {
        return false;
    }
    // Base Case:
    if (feedback_vertex_set.size() == target_size) {

        // Make a copy of the adj_mat
        vector<vector<int> > adj_mat_copy;
        for (int i=0; i<adj_mat.size(); i++) {
            vector<int> new_row;
            for (int j=0; j<adj_mat[0].size(); j++) {
                new_row.push_back(adj_mat[i][j]);
            }
            adj_mat_copy.push_back(new_row);
        }

        // Remove elements in the set elements_to_remove from the adj_mat_copy
        for (int i=0; i<feedback_vertex_set.size(); i++) {
            disconnect_vertex(adj_mat_copy, feedback_vertex_set[i]);
        }

        // Check if the adj_mat_copy has no cycle. If yes return true.
        if (no_cycle_detected(adj_mat_copy)) {
            base_case_set = feedback_vertex_set;
            return true;
        }

        return false;
    }

    // Recursive call:
    feedback_vertex_set.push_back(elements_to_check[start]);
    bool option_1 = feedback_set_possible(adj_mat, elements_to_check, start+1, feedback_vertex_set, target_size);

    feedback_vertex_set.pop_back();
    bool option_2 = feedback_set_possible(adj_mat, elements_to_check, start+1, feedback_vertex_set, target_size);

    if (option_1 || option_2) {
        return true;
    }

    return false;
}

// Function to run exact algorithm.
// If heuristic is true the time optization heurisitic will apply.
vector<int> feedback_vertex_set_fast(vector<vector<int> > adj_mat, bool heuristic) {
    int n = adj_mat.size();
    
    // Reducing the graph such that the feedback vertex size remains the same:
    set<int> disconnected_elements = clear(adj_mat);

    // Now we can find the feedback vertex set by trying all elements:
    vector<int> elements_to_check;
    for (int i=0; i<n; i++) {
        if (disconnected_elements.find(i) == disconnected_elements.end()) {
            elements_to_check.push_back(i);
        }
    }

    int k = elements_to_check.size();

    if (heuristic) {
        map<int, int> node_degree_map = find_degrees(adj_mat, elements_to_check);
        // Now we have element -> degree map
        vector<pair<int, int> > element_degree_pair_vector;

        for (int i=0; i<node_degree_map.size(); i++) {
            element_degree_pair_vector.push_back(make_pair(elements_to_check[i], node_degree_map[elements_to_check[i]]));
        }
        vector<pair<int, int> > vec = element_degree_pair_vector;
        sort(vec.begin(), vec.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a.second > b.second; });
        element_degree_pair_vector = vec;

        for (int i=0; i<element_degree_pair_vector.size(); i++) {
            elements_to_check[i] = element_degree_pair_vector[i].first;
        }
    }
    // Now we need to find all combination for elements to check and find if any no cycle combination is available.
    int min_set_size = -1;
    vector<int> feedback_vertex_set;
    for (int i = 0; i<k; i++) {
        feedback_vertex_set.clear();
        if (feedback_set_possible(adj_mat, elements_to_check, 0, feedback_vertex_set, i)) {
            min_set_size = i;
            break;
        }
    }

    if (min_set_size == -1) {
        return elements_to_check;
    }

    return base_case_set;
}


int *node_degree_register;
vector<vector<int> > adj_mat_approx_parallel;
vector<int> required_elements_parallel;
// Function find degree of all nodes between start and end (both included) in a parallel fasion.
void degree_thread(int start_node, int end_node) {
    set<int> required_elements_set;
    for (int i=0; i<required_elements_parallel.size(); i++) {
        required_elements_set.insert(required_elements_parallel[i]);
    }
    int n = adj_mat_approx_parallel.size();

    for (int node = start_node; node<=end_node; node++) {
        if (required_elements_set.find(node) != required_elements_set.end()) {
            for (int i=0; i<n; i++) {
                if (adj_mat_approx_parallel[node][i] == 1) {
                    node_degree_register[node]++;
                }
            }
        }
    }
}
// Find degree parallel finds the degree of all required_elements in a parallel fasion.
// It uses the above thread function.
map<int, int> find_degrees_parallel(vector<vector<int> > &adj_mat, vector<int> &required_elements, int p) {
    int n = adj_mat.size();
    map<int, int> node_degree_map;

    adj_mat_approx_parallel = adj_mat;
    required_elements_parallel = required_elements;

    node_degree_register = new int[n];
    for (int i=0; i<n; i++) {
        node_degree_register[i] = 0;
    }

    int to_add = (n/p)+1;
    int thread_ctr = 0;
    for (int start=0; start < n; start+=to_add) {
        int end = start + to_add -1;
        if (end >= n) {
            end = n-1;
        }
        thread_ctr++;
    }
    thread *threads = new thread[thread_ctr];
    to_add = (n/p) + 1;
    thread_ctr = 0;
    for (int start=0; start < n; start+=to_add) {
        int end = start + to_add -1;
        if (end >= n) {
            end = n-1;
        }
        threads[thread_ctr] = thread(degree_thread, start, end);
        thread_ctr++;
    }

    // Joining threads
    for (int i = 0; i < thread_ctr; i++) {
        threads[i].join();
    }

    for (int i=0; i<required_elements.size(); i++) {
        int node = required_elements[i];
        node_degree_map[node] = node_degree_register[node];
    }
    return node_degree_map;
}


// Approximation function to find the feedback vertex set.
vector<int> feedback_vertex_approximation(vector<vector<int> > adj_mat, bool parallelize, int p) {
    int n = adj_mat.size();
    
    // Reducing the graph such that the feedback vertex size remains the same:
    set<int> disconnected_elements = clear(adj_mat);

    // Now we can find the feedback vertex set by trying all elements:
    vector<int> elements_to_check;
    for (int i=0; i<n; i++) {
        if (disconnected_elements.find(i) == disconnected_elements.end()) {
            elements_to_check.push_back(i);
        }
    }

    int k = elements_to_check.size();

    if (k == 0) {
        vector<int> empty;
        return empty;
    }

    vector<int> feedback_vertex_set;
    auto start_time = std::chrono::steady_clock::now();
    for (int i=0; i<n; i++) {
        auto current_time = std::chrono::steady_clock::now();
        int time_difference = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
        if (time_difference > time_limit) {
            has_timed_out = true;
            break;
        }
        map<int, int> node_degree_map;
        if (parallelize) {
            node_degree_map = find_degrees_parallel(adj_mat, elements_to_check, p);
        } else {
            node_degree_map = find_degrees(adj_mat, elements_to_check);
        }

        for (int j=0; j<elements_to_check.size(); j++) {
        }
        // Now we have element -> degree map
        int max_degree = node_degree_map[elements_to_check[0]];
        int element_max_degree = elements_to_check[0];

        for (int j=0; j<elements_to_check.size(); j++) {
            if (node_degree_map.find(elements_to_check[j]) != node_degree_map.end()) {
                if (node_degree_map[elements_to_check[j]] > max_degree) {
                    max_degree = node_degree_map[elements_to_check[j]];
                    element_max_degree = elements_to_check[j];
                }
            }
        }

        if (max_degree == 0) {
            break;
        }

        if (no_cycle_detected(adj_mat)) {
            break;
        }

        feedback_vertex_set.push_back(element_max_degree);
        disconnect_vertex(adj_mat, element_max_degree);
    }
    return feedback_vertex_set;
}


// Function to find the value of k without calling any other function.
int find_k(vector<vector<int> > adj_mat) {

    int n = adj_mat.size();
    
    // Reducing the graph such that the feedback vertex size remains the same:
    set<int> disconnected_elements = clear(adj_mat);

    // Now we can find the feedback vertex set by trying all elements:
    vector<int> elements_to_check;
    for (int i=0; i<n; i++) {
        if (disconnected_elements.find(i) == disconnected_elements.end()) {
            elements_to_check.push_back(i);
        }
    }

    int k = elements_to_check.size();
    return k;
}


int minimum_graph_size_parallel_fast = -1;
int loop_counter_parallel_fast = 0;
vector<vector<int> > adj_mat_parallel_fast;
vector<int> elements_to_check_parallel_fast;
vector<int> feedback_set_parallel_exact_fast;
// Thread to find the feedback vertex set: exact solution. One of the threads (running) parallely gives the exact solution.
void thread_function_fast(int start, int end, int k) {
    // Making own copies
    vector<vector<int> > adj_mat;
    adj_mat = adj_mat_parallel_fast;
    vector<int> elements_to_check;
    elements_to_check = elements_to_check_parallel_fast;

    double two_power_k = pow(2, k);
    for (int count_in_feedback_set = start; count_in_feedback_set<=end; count_in_feedback_set++) {
        // Making a copy of the adjacency matrix for parallelization:
        if (count_in_feedback_set == 0) {
            continue;
        }
        for (int bit_number = pow(2, count_in_feedback_set)-1; bit_number<=two_power_k; bit_number=find_next_num_same_setbits(bit_number)) {
            loop_counter_parallel_fast++;

            if (bit_number == 0) {
                continue;
            }

            vector<vector<int> > adj_mat_copy;
            for (int i=0; i<adj_mat.size(); i++) {
                vector<int> new_row;
                for (int j=0; j<adj_mat[0].size(); j++) {
                    new_row.push_back(adj_mat[i][j]);
                }
                adj_mat_copy.push_back(new_row);
            }
            // Now we have made the copy of our adjacency matrix.

            // Removing the nodes with indices in the array elements_to_check = bit indices == 1 in bit_number.
            int temp_bit_number = bit_number;
            int number_of_nodes_removed = 0;
            vector<int> possible_set;
            for (int i=0; i<k; i++) {
                if(temp_bit_number%2 == 1) {
                    number_of_nodes_removed++;
                    disconnect_vertex(adj_mat_copy, elements_to_check[i]);
                    possible_set.push_back(elements_to_check[i]);
                }
                temp_bit_number /= 2;
            }

            if (no_cycle_detected(adj_mat_copy)) {
                if (number_of_nodes_removed < minimum_graph_size_parallel_fast) {
                    minimum_graph_size_parallel_fast = number_of_nodes_removed;
                    feedback_set_parallel_exact_fast = possible_set;
                }
                break;
            }
        }
    }
}

// Function to find the feedback vertex set in a parallel fasion.
vector<int> feedback_vertex_set_exact_parallel_fast(vector<vector<int> > adj_mat, int p, bool heuristic) {

    int n = adj_mat.size();
    
    // Reducing the graph such that the feedback vertex size remains the same:
    set<int> disconnected_elements = clear(adj_mat);

    // Now we can find the feedback vertex set by trying all elements:
    vector<int> elements_to_check;
    for (int i=0; i<n; i++) {
        if (disconnected_elements.find(i) == disconnected_elements.end()) {
            elements_to_check.push_back(i);
        }
    }

    // Copying vectors to global variables: (These variables remain the save in all threads).
    adj_mat_parallel_fast = adj_mat;
    elements_to_check_parallel_fast = elements_to_check;

    if (heuristic) {
        map<int, int> node_degree_map = find_degrees(adj_mat, elements_to_check);
        // Now we have element -> degree map
        vector<pair<int, int> > element_degree_pair_vector;

        for (int i=0; i<node_degree_map.size(); i++) {
            element_degree_pair_vector.push_back(make_pair(elements_to_check[i], node_degree_map[elements_to_check[i]]));
        }
        vector<pair<int, int> > vec = element_degree_pair_vector;
        sort(vec.begin(), vec.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a.second > b.second; });
        element_degree_pair_vector = vec;

        for (int i=0; i<element_degree_pair_vector.size(); i++) {
            elements_to_check[i] = element_degree_pair_vector[i].first;
        }
    }

    int k = elements_to_check.size();
    minimum_graph_size_parallel_fast = k;

    // Creating threads
    int to_add = (k/p)+1;
    int thread_ctr = 0;
    for (int to_remove = 0; to_remove<k; to_remove += to_add) {
        thread_ctr++;
    }

    thread *threads = new thread[thread_ctr];

    to_add = (k/p)+1;
    thread_ctr = 0;
    for (int to_remove = 0; to_remove<k; to_remove += to_add) {
        threads[thread_ctr] = thread(thread_function_fast, to_remove, to_remove+to_add-1, k);
        thread_ctr++;
    }

    // Joining threads
    for (int i = 0; i < thread_ctr; i++) {
        threads[i].join();
    }

    if (minimum_graph_size_parallel_fast == -1) {
        return elements_to_check;
    }

    return feedback_set_parallel_exact_fast;
}

// It is a intutive heuristic algorithm
vector<int> approximate_heuristic(vector<vector<int> > &adj_mat) {

    vector<int> elements_to_check;
    int n = adj_mat.size();
    for (int i=0; i<n; i++) {
        elements_to_check.push_back(i);
    }
    map<int, int> node_degree_map = find_degrees(adj_mat, elements_to_check);

    // Now we have element -> degree map
    // Sorting the nodes in increasing order of degree
    vector<pair<int, int> > element_degree_pair_vector;

    for (int i=0; i<node_degree_map.size(); i++) {
        element_degree_pair_vector.push_back(make_pair(elements_to_check[i], node_degree_map[elements_to_check[i]]));
    }

    vector<pair<int, int> > vec = element_degree_pair_vector;
    sort(vec.begin(), vec.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a.second < b.second; });
    element_degree_pair_vector = vec;

    for (int i=0; i<element_degree_pair_vector.size(); i++) {
        elements_to_check[i] = element_degree_pair_vector[i].first;
    }

    vector<int> feedback_vertex_set;
    set<int> nodes_in_graph;

    vector<vector<int> > adj_mat_without_cycle(n, vector<int>(n, 0));

    auto start_time = std::chrono::steady_clock::now();
    for (int i=0; i<n; i++) {
        auto current_time = std::chrono::steady_clock::now();
        int time_difference = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
        if (time_difference > time_limit) {
            has_timed_out = true;
            break;
        }
        // Adding ith node:
        add_vertex(adj_mat, adj_mat_without_cycle, i, nodes_in_graph);
        if (!no_cycle_detected(adj_mat_without_cycle)) {
            // Cycle detected!
            // Removing ith node:
            disconnect_vertex(adj_mat_without_cycle, i);
            // Putting ith vertex in feedback vertex set.
            feedback_vertex_set.push_back(i);
        } else {
            // If no cycle: keep the node in graph.
            nodes_in_graph.insert(i);
        }
    }
    return feedback_vertex_set;
}
