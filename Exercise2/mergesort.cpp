#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <regex>
#include <filesystem>
#include <stdint.h>

#include "mergesort.h"
#include "random_numbers.h"
#include "utils.h"
#include <argparse/argparse.hpp>



int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("MergeSort");

    program.add_argument("n_numbers")
        .help("How many numbers to generate and sort")
        .scan<'i', int>();

    program.add_argument("n_runs")
        .help("How many runs to perform")
        .scan<'i', int>();

    program.add_argument("-p", "--print")
        .help("Print vector before and after mergesort")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("-t", "--test")
        .help("test sorting by comparing with stl sort")
        .default_value(false)
        .implicit_value(true);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    long long n_numbers = program.get<int>("n_numbers");
    long long n_runs = program.get<int>("n_runs");
    bool print_vector = program["--print"] == true;
    bool testmode = program["--test"] == true;
    std::vector<long long> sort_times;


    if (n_numbers <= 0) {
        std::cerr << "arg 'n_numbers' must be positive" << std::endl;
        return 1;
    }

    if (n_runs <= 0) {
        std::cerr << "arg 'n_runs' must be positive" << std::endl;
        return 1;
    }


    for(int i = 0; i < n_runs; i++) {
        std::vector<int> v = gen_non_duplicate_random_integers(n_numbers);

        std::vector<int> v_copy;
        if(testmode) {
            v_copy = v;
            std::sort(std::begin(v_copy), std::end(v_copy));
        }

        if(print_vector) {
            std::cout << "Vector prior to Sorting" << std::endl;
            print_vec(v);
        }

        auto start_sort = std::chrono::high_resolution_clock::now();
        mergeSort(v, 0, v.size() - 1);
        auto stop_sort = std::chrono::high_resolution_clock::now();

        if(print_vector) {
            std::cout << "Vector after Sorting" << std::endl;
            print_vec(v);
        }

        auto duration_sort = std::chrono::duration_cast<std::chrono::microseconds>(stop_sort - start_sort);
        std::cout << "Sorting took " << duration_sort.count() << " microseconds" << std::endl;
        sort_times.push_back(duration_sort.count());

        if(testmode) {
            if(v != v_copy) {
                throw std::runtime_error("Vector not equal to stl sorted vector!");
            }
        }
    }



    return 0;

}