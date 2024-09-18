//
// Created by Nils Schnakenberg on 16.11.23.
//

#include <iostream>
#include <fstream>

#include "io_utils.h"
#include "random_number_generator.h"

long long get_file_size(std::string &filename) {
    std::fstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        // Get the file size by reading the position of the file pointer
        std::streampos file_size = file.tellg();
        file.close();
        return file_size;
    } else {
        std::cerr << "Unable to open the file." << std::endl;
        return -1;
    }
}


bool write_integers_at_offset(std::string &filename, std::vector<int> &integers, long long offset) {
    // open file in binary mode to write some integers into it
    std::fstream outFile(filename, std::ios::out | std::ios::binary | std::ios::in);

    // check file could be opened
    if (!outFile.is_open()) {
        std::cerr << "Failed to open the file for writing." << filename << std::endl;
        return false;
    }

    if (offset) {
        outFile.seekg(offset);
    }

    // Write the vector of integers to the file
    outFile.write(reinterpret_cast<char *>(integers.data()), integers.size() * sizeof(int));

    outFile.close();

    if (outFile.fail()) {
        std::cerr << "error writing to file" << std::endl;
        return false;
    }

    return true;
}

 void write_demo_files(std::string &filename, long long n_integer, long long block_size) {

    long long n_integers_per_block = block_size/sizeof(int);

    long long created_integers = 0;

    while (created_integers < n_integer) {
        long long integers_to_create = std::min(n_integers_per_block, n_integer - created_integers);
        auto random_integers = gen_random_integers(integers_to_create, created_integers * 4);
        write_integers_at_offset(filename, random_integers, created_integers * 4);
        created_integers += integers_to_create;
    }
}

std::vector<int> read_integers(std::string &filename, long long offset) {
    std::ifstream in_file(filename, std::ios::binary);

    int read_int;
    std::vector<int> read_integers;

    if (offset) {
        in_file.seekg(offset);
    }

    while(!in_file.eof()) {
        in_file.read(reinterpret_cast<char *>(&read_int), sizeof(int));
        read_integers.push_back(read_int);
    }
    read_integers.pop_back();

    in_file.close();
    return read_integers;
}

std::vector<int> read_block(std::string &filename, long long offset, long long blocksize) {
    std::fstream in_file(filename, std::ios::in | std::ios::binary);

    long long n_integers = blocksize / sizeof(int);

    if(blocksize % sizeof(int) != 0) {
        std::cerr << "Bad blocksize" << std::endl;
        throw std::exception();
    }

    if (offset) {
        in_file.seekg(offset);
    }

    int *read_integers = new int[n_integers];
    in_file.read(reinterpret_cast<char *>(read_integers), blocksize);

    if(in_file.eof()) {
        in_file.close();

        std::fstream in_file2(filename, std::ios::in | std::ios::binary);

        if (offset) {
            in_file2.seekg(offset);
        }

        auto in_file_size = get_file_size(filename);
        auto read_size = in_file_size - offset;

        if(read_size <= 0) {
            std::vector<int> res;
            return res;
        }

        n_integers = read_size/sizeof(int);

        int *read_integers2 = new int[n_integers];

        in_file2.read(reinterpret_cast<char *>(read_integers2), read_size);
        std::vector<int> read_integers_vec2(read_integers2, read_integers2 + n_integers);
        in_file2.close();


        delete[] read_integers;
        delete[] read_integers2;

        return read_integers_vec2;
    }

    std::vector<int> read_integers_vec(read_integers, read_integers + n_integers);

    in_file.close();

    delete[] read_integers;

    return read_integers_vec;
}

void print_vec(std::vector<int> data) {
    for(auto i: data) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}