#include <iostream>
#include <vector>
#include <regex>
#include <numeric>
#include <chrono>
#include <filesystem>

#include "lib/mergesort.h"
#include "lib/random_number_generator.h"
#include "lib/io_utils.h"
#include "lib/em_mergesort.h"

// Function to calculate the average of a vector of long long integers
long long average(std::vector<long long> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<long long>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}


int main(int argc, char *argv[]) {

    // Command line argument validation
    if(argc != 5) {
        std::cout << "USAGE: em_mergesort [blocksize] [ram_share] [n_runs] [test]" << std::endl;
        return (1);
    }

    // File names for data storage
    std::string filename = "demo.bin";
    std::string filename2 = "demo2.bin";
    std::string test_backup = "demo_back.bin";

    // Parsing command line arguments
    double ram_share = std::stod(argv[2]);
    auto ram_size = getSystemRamSize();
    long n_numbers = ram_share * ram_size / sizeof(int);
    int n_runs = atoi(argv[3]);
    bool test = atoi(argv[4]);

    // Vector to store sorting times for multiple runs.
    std::cout << test << std::endl;

    long long blocksize = atoi(argv[1]);// 256; // 4 integers

    std::vector<long long> sort_times;

    std::cout << "RAM share: " << ram_share << std::endl;
    std::cout << "RAM size: " << ram_size << std::endl;
    std::cout << "n Integers: " << n_numbers << std::endl;

    for(int i = 0; i < n_runs; i++) {

        std::cout << "Starting run: " << i+1 << " of " << n_runs << std::endl;

        // (re)create file for a fresh start
        std::remove(filename.c_str());
        std::remove(filename2.c_str());
        std::remove(test_backup.c_str());
        fopen(filename.c_str(), "wb");
        fopen(filename2.c_str(), "wb");

        std::cout << "Writing demo files" << std::endl;
        write_demo_files(filename, n_numbers, blocksize);
        std::cout << "Wrote demo files" << std::endl;

        if(test) {
            std::cout << "test enabled" << std::endl;
            std::cout << "copying original vector" << std::endl;
            std::filesystem::copy_file(filename, test_backup);
            std::cout << "copied original vector" << std::endl;
        }

        std::cout << "Start Sorting" << std::endl;
        auto start_sort = std::chrono::high_resolution_clock::now();

        // External Memory MergeSort operations
        sort_partitions(filename, filename, blocksize);
        std::cout << "partitioned" << std::endl;
        merge_partitions(filename, filename2, blocksize, blocksize);

        auto stop_sort = std::chrono::high_resolution_clock::now();
        std::cout << "Finished Sorting" << std::endl;

        auto duration_sort = std::chrono::duration_cast<std::chrono::microseconds>(stop_sort - start_sort);
        std::cout << "Sorting took " << duration_sort.count() << " microseconds" << std::endl;
        sort_times.push_back(duration_sort.count());




        if(test) {
            // Verification of sorting correctness
            auto orig_vector = read_integers(test_backup, 0);
            std::sort(orig_vector.begin(), orig_vector.end());
            auto final_vector = read_integers(filename, 0);
            std::cout << "orig_vector.size() " << orig_vector.size() << std::endl;
            std::cout << "final_vector.size() " << final_vector.size() << std::endl;

            std::cout << "orig_vector[0] " << orig_vector[0] << std::endl;
            std::cout << "final_vector[0] " << final_vector[0] << std::endl;
            std::cout << "orig_vector[orig_vector.size()-1] " << orig_vector[orig_vector.size()-1] << std::endl;
            std::cout << "final_vector[orig_vector.size()-1] " << final_vector[orig_vector.size()-1] << std::endl;
            if(orig_vector != final_vector) {
                throw std::runtime_error("Errormessage!");
            }
        }
    }

    // Display the average sorting duration across multiple runs
    std::cout << "Average sorting duration: " << average(sort_times) << std::endl;

    return 0;

}