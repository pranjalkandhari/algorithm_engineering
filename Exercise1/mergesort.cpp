#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <regex>
#include <filesystem>

#include "lib/mergesort.h"
#include "lib/random_number_generator.h"
#include "lib/io_utils.h"

// Function to calculate the average of a vector of long long values.
long long average(std::vector<long long> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<long long>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}


int main(int argc, char *argv[]) {

     // Check if the correct number of command-line arguments is provided
    if(argc != 4) {
        std::cout << "USAGE: mergesort [ram_share] [n_runs] [test]" << std::endl;
        return (1);
    }

    // Set up file names and parameters
    std::string filename = "demo.bin";
    std::string filename2 = "demo2.bin";
    std::string test_backup = "demo_back.bin";

    double ram_share = std::stod(argv[1]);
    auto ram_size = getSystemRamSize();
    long n_numbers = ram_share * ram_size / sizeof(int);
    int n_runs = atoi(argv[2]);
    bool test = atoi(argv[3]);

    std::cout << test << std::endl;
    // Vector to store sorting times
    std::vector<long long> sort_times;

    std::cout << "RAM share: " << ram_share << std::endl;
    std::cout << "RAM size: " << ram_size << std::endl;
    std::cout << "n Integers: " << n_numbers << std::endl;

    // Run sorting for the specified number of times
    for(int i = 0; i < n_runs; i++) {

        std::cout << "Starting run: " << i+1 << " of " << n_runs << std::endl;

        // (Re)create file for a fresh start
        std::remove(filename.c_str());
        std::remove(filename2.c_str());
        std::remove(test_backup.c_str());
        fopen(filename.c_str(), "wb");
        fopen(filename2.c_str(), "wb");

        std::cout << "Writing demo files" << std::endl;
        write_demo_files(filename, n_numbers, 268435456);
        std::cout << "Wrote demo files" << std::endl;

        if(test) {
            std::cout << "test enabled" << std::endl;
            std::cout << "copying original vector" << std::endl;
            std::filesystem::copy_file(filename, test_backup);
            std::cout << "copied original vector" << std::endl;
        }

        // Read data from the file
        auto data = readIntegersFromFile(filename);
        int n = data.size();

        std::cout << "Start Sorting" << std::endl;
        auto start_sort = std::chrono::high_resolution_clock::now();
        // Perform MergeSort on the data
        mergeSort(data, 0, n-1);
        //write_integers_at_offset(filename2, data, 0);
        auto stop_sort = std::chrono::high_resolution_clock::now();
        std::cout << "Finished Sorting" << std::endl;

        // Calculate the duration of the sorting process
        auto duration_sort = std::chrono::duration_cast<std::chrono::microseconds>(stop_sort - start_sort);
        std::cout << "Sorting took " << duration_sort.count() << " microseconds" << std::endl;
        sort_times.push_back(duration_sort.count());

        // Perform testing if enabled
        if(test) {
            auto orig_vector = read_integers(test_backup, 0);
            std::sort(orig_vector.begin(), orig_vector.end());
            std::cout << "orig_vector.size() " << orig_vector.size() << std::endl;
            std::cout << "data.size() " << data.size() << std::endl;

            std::cout << "orig_vector[0] " << orig_vector[0] << std::endl;
            std::cout << "data[0] " << data[0] << std::endl;
            std::cout << "orig_vector[orig_vector.size()-1] " << orig_vector[orig_vector.size()-1] << std::endl;
            std::cout << "data[orig_vector.size()-1] " << data[orig_vector.size()-1] << std::endl;
            // Check if the sorted vector matches the expected result
            if(orig_vector != data) {
                throw std::runtime_error("Errormessage!");
            }
        }
    }
    // Calculate and print the average sorting duration.
    std::cout << "Average sorting duration: " << average(sort_times) << std::endl;

    return 0;

}
