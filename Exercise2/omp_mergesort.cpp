#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <regex>
#include <filesystem>

#include "mergesort.h"
#include "random_numbers.h"
#include "utils.h"
#include <argparse/argparse.hpp>


int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("OMPMergeSort");

    program.add_argument("n_numbers")
        .help("count of numbers to generate and sort")
        .scan<'i', int>();

    program.add_argument("n_runs")
            .help("How many runs to perform")
            .scan<'i', int>();

    program.add_argument("n_proc")
            .help("count of threads(processor cores) to use")
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
    int n_proc = program.get<int>("n_proc");
    long long n_runs = program.get<int>("n_runs");
    bool print_vector = program["--print"] == true;
    bool testmode = program["--test"] == true;
    std::vector<long long> sort_times;


    if (n_numbers <= 0) {
        std::cerr << "arg 'n_numbers' must be positive" << std::endl;
        return 1;
    }

    for(int i = 0; i < n_runs; i++) {

        std::cerr << i << " ";

        auto v = gen_non_duplicate_random_integers(n_numbers);

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
        ompMergeSort(v, n_proc, 0, v.size() - 1);
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

    std::cout << "Average sorting duration: " << average(sort_times) << std::endl;

    return 0;

}