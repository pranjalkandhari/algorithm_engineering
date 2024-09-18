//
// Created by Nils Schnakenberg on 16.11.23.
//

#ifndef ALGORITHM_ENGINEERING_IO_UTILS_H
#define ALGORITHM_ENGINEERING_IO_UTILS_H

#include <string>
#include <iostream>
#include <vector>


long long get_file_size(std::string &filename);

bool write_integers_at_offset(std::string &filename, std::vector<int> &integers, long long offset);

void write_demo_files(std::string &filename, long long n_integer);

void write_demo_files(std::string &filename, long long n_integer, long long block_size);

std::vector<int> read_integers(std::string &filename, long long offset);

std::vector<int> read_block(std::string &filename, long long offset, long long blocksize);

void print_vec(std::vector<int> data);

#endif //ALGORITHM_ENGINEERING_IO_UTILS_H
