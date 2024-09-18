#include "random_number_generator.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <thread>
#include <iomanip>
#include <sstream>
#include <climits>

// Function to get system RAM size in bytes
long long getSystemRamSize() {
    // Replace with platform-specific code to get system RAM size
    return (long long)2 * (long long)1024 * (long long)1024 * (long long)1024; // 16 GB as a placeholder
}

// Function to generate random integers and save them to a file
void generateRandomIntegersFile(const std::string& filename, long long fileSizeInBytes) {

    std::ofstream outputFile(filename, std::ios::binary);

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open the file for writing." << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);

    auto int_size = sizeof(int);
    auto elements_to_generate = fileSizeInBytes / int_size;

    int x;
    for (long i = 0; i < elements_to_generate; i++) {
        //x = dist(mt);
        x = elements_to_generate - i;
        outputFile.write(reinterpret_cast<char*>(&x), sizeof(dist(mt)));
    }

    outputFile.close();
}

void generateRandomIntegersRAM(const std::string& filename_base, std::vector<double> ram_percentages) {
    std::vector<std::thread> threads;

    long long ram_size = getSystemRamSize();
    std::cout << "RAM size: " << ram_size << std::endl;

    for (double percentage : ram_percentages) {
        long long file_size = percentage * getSystemRamSize() / 100.0;
        auto int_size = sizeof(int);
        auto elements_to_generate = file_size / int_size;

        std::cout << "Creating File with size  " << elements_to_generate << " of size " << file_size << " for ram percentage " << percentage << std::endl;

        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << percentage;
        std::string percentage_format = stream.str();

        std::string filename = filename_base + "_" + percentage_format  + "_" + "percent.dat";
        threads.emplace_back(generateRandomIntegersFile, filename, file_size);
    }

    // Join all the threads to ensure they finish before exiting
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Files creation completed." << std::endl;
}

std::vector<int> readIntegersFromFile(const std::string& filename) {
    std::ifstream inputFile(filename, std::ios::binary);

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file for reading." << std::endl;
        throw std::runtime_error("Failed to open the file for reading.");
    }

    // Read integers from the file into a vector
    std::vector<int> randomIntegers;
    int value;
    while (inputFile.read(reinterpret_cast<char*>(&value), sizeof(value))) {
        randomIntegers.push_back(value);
    }

    inputFile.close();

    return randomIntegers;
}


std::vector<int> gen_random_integers(long long n_integers, int min) {
    // initialize random generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);

    std::vector<int> random_integers;
    random_integers.reserve(n_integers);

    for (int i = 0; i < n_integers; ++i) {
        int random_int = dist(mt);
        random_integers.push_back(random_int);
    }
    return random_integers;
}