// This file is to run the algorithms.
// Algorithms are in algorithm.h
#include <iostream>
#include <cstring>
#include <vector>
#include <filesystem>
#include <fstream>
#include <chrono>
#include "algorithms.h"
using namespace std;
namespace fs = std::filesystem;


string INPUT_DATA_PATH = "input_data/";
int K_LIMIT_EXP = 29; // Limit of K for exponential algorithms
bool RUN_EXP_ALGORITHMS = true; // true: Run all algorithms. true: Only polynomial time algorithms.


void print_vector(vector<int> array) {
    for (int i=0; i<array.size(); i++) {
        cout<<array[i]<<" ";
    }
    cout<<endl;
}


int main() {

    std::string directory = INPUT_DATA_PATH;

    vector<string> LIST_OF_INPUT_FILES;

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            LIST_OF_INPUT_FILES.push_back(entry.path().filename().string()); // Add the filename to the vector
        }
    }

    // Running exact algorithm for all graphs:
    int lower_limit = 0;
    map<string, int> file_k_map;
    for (int k_limit = 1; k_limit<=K_LIMIT_EXP && RUN_EXP_ALGORITHMS; k_limit += 1, lower_limit += 1) {
        //LIST_OF_INPUT_FILES = {"graph_03.txt"};
        for(string input_file_name: LIST_OF_INPUT_FILES) {
            string file_path = INPUT_DATA_PATH + input_file_name;

            ifstream file(file_path);

            int k;
            if (file_k_map.find(file_path) != file_k_map.end()) {
                k = file_k_map[file_path];
                if (k > k_limit || k<=lower_limit) {
                    continue;
                }
            }

            if (file.is_open()) { // Check if the file is opened successfully
                
                int n,e;
                file>>n>>e;

                vector<vector<int>> adj_mat(n, vector<int>(n, 0));

                for (int i=0; i<e; i++) {
                    int a,b;
                    file>>a>>b;
                    adj_mat[a][b] = 1;
                    adj_mat[b][a] = 1;
                }

                if (file_k_map.find(file_path) == file_k_map.end()) {
                    k = find_k(adj_mat);
                    file_k_map[file_path] = k;
                }
                
                if (k > k_limit || k<=lower_limit) {
                    continue;
                }
                cout<<file_path<<endl;
                cout<<"K: "<<k<<endl;

                // Calling for exact algorithm
                auto start_time = std::chrono::high_resolution_clock::now();
                vector<int> feedback_vertex_set = feedback_vertex_set_fast(adj_mat, false);
                auto end_time = std::chrono::high_resolution_clock::now();
                cout<<"EXACT:"<<endl;
                print_vector(feedback_vertex_set);
                std::chrono::duration<double, std::milli> duration = end_time - start_time;
                std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
                cout<<"------"<<endl;
                
                // Calling for time optimized heuristic algorithm
                start_time = std::chrono::high_resolution_clock::now();
                feedback_vertex_set = feedback_vertex_set_fast(adj_mat, true);
                end_time = std::chrono::high_resolution_clock::now();
                cout<<"HEURISTIC: "<<endl;
                print_vector(feedback_vertex_set);
                duration = end_time - start_time;
                std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
                cout<<"------"<<endl;

                // Calling for parallelized exact algorithm
                start_time = std::chrono::high_resolution_clock::now();
                feedback_vertex_set = feedback_vertex_set_exact_parallel_fast(adj_mat, 8, false);
                end_time = std::chrono::high_resolution_clock::now();
                cout<<"EXACT PARALLEL: "<<endl;
                print_vector(feedback_vertex_set);
                duration = end_time - start_time;
                std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
                cout<<"------"<<endl;

                // Calling for parallelized time optimized heuristic algorithm
                start_time = std::chrono::high_resolution_clock::now();
                feedback_vertex_set = feedback_vertex_set_exact_parallel_fast(adj_mat, 8, true);
                end_time = std::chrono::high_resolution_clock::now();
                cout<<"EXACT PARALLEL with Heuristic: "<<endl;
                print_vector(feedback_vertex_set);
                duration = end_time - start_time;
                std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
                cout<<"------"<<endl;

                // Calling for approximation algorithm:
                start_time = std::chrono::high_resolution_clock::now();
                feedback_vertex_set = feedback_vertex_approximation(adj_mat, false, 1);
                end_time = std::chrono::high_resolution_clock::now();
                cout<<"APPROXIMATION: "<<endl;
                print_vector(feedback_vertex_set);
                duration = end_time - start_time;
                std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
                cout<<"------"<<endl;

                // Calling for parallelized approximation algorithm:
                start_time = std::chrono::high_resolution_clock::now();
                feedback_vertex_set = feedback_vertex_approximation(adj_mat, true, 8);
                end_time = std::chrono::high_resolution_clock::now();
                cout<<"APPROXIMATION PARALLEL: "<<endl;
                print_vector(feedback_vertex_set);
                duration = end_time - start_time;
                std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
                cout<<"------"<<endl;

                cout<<"-----------------------"<<endl;

                file.close(); // Close the file after reading
            } else {
                std::cerr << "Unable to open file!" << std::endl; // Print error message if file opening fails
                return 1; // Return error code
            }
        }
    }

    cout<<"POLYNOMIAL TIME ALGORITHMS: "<<endl;
    for(string input_file_name: LIST_OF_INPUT_FILES) {
            string file_path = INPUT_DATA_PATH + input_file_name;

            ifstream file(file_path);

            if (file.is_open()) { // Check if the file is opened successfully
                
                int n,e;
                file>>n>>e;

                vector<vector<int>> adj_mat(n, vector<int>(n, 0));

                for (int i=0; i<e; i++) {
                    int a,b;
                    file>>a>>b;
                    adj_mat[a][b] = 1;
                    adj_mat[b][a] = 1;
                }
                has_timed_out = false;

                // Calling for approximation algorithm:
                auto start_time = std::chrono::high_resolution_clock::now();
                vector<int> feedback_vertex_set;
                auto end_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duration = end_time - start_time;

                start_time = std::chrono::high_resolution_clock::now();
                feedback_vertex_set = feedback_vertex_approximation(adj_mat, false, 1);
                if (has_timed_out) {
                    has_timed_out = false;
                    // cout<<"TIME OUT"<<endl;
                    // cout<<"-----------------------"<<endl;
                    continue;
                }
                cout<<file_path<<endl;

                end_time = std::chrono::high_resolution_clock::now();
                cout<<"APPROXIMATION: "<<endl;
                print_vector(feedback_vertex_set);
                cout<<feedback_vertex_set.size()<<endl;
                duration = end_time - start_time;
                std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
                cout<<"------"<<endl;

                // Calling for parallelized approximation algorithm:
                start_time = std::chrono::high_resolution_clock::now();
                feedback_vertex_set = feedback_vertex_approximation(adj_mat, true, 8);
                end_time = std::chrono::high_resolution_clock::now();
                cout<<"APPROXIMATION PARALLEL: "<<endl;
                print_vector(feedback_vertex_set);
                cout<<feedback_vertex_set.size()<<endl;
                duration = end_time - start_time;
                std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
                cout<<"------"<<endl;

                // Calling for approximation heuristic algorithm:
                start_time = std::chrono::high_resolution_clock::now();
                feedback_vertex_set = approximate_heuristic(adj_mat);
                end_time = std::chrono::high_resolution_clock::now();
                cout<<"HEURISTIC 2: "<<endl;
                if (has_timed_out) {
                    has_timed_out = false;
                    cout<<"TIMEOUT"<<endl;
                    cout<<"-----------------------"<<endl;
                    continue;
                }
                print_vector(feedback_vertex_set);
                cout<<feedback_vertex_set.size()<<endl;
                duration = end_time - start_time;
                std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
                cout<<"------"<<endl;

                cout<<"-----------------------"<<endl;

                file.close(); // Close the file after reading
            } else {
                std::cerr << "Unable to open file!" << std::endl; // Print error message if file opening fails
                return 1; // Return error code
            }
            // break;
        }


}
