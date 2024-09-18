//
// Created by Nils Schnakenberg on 07.11.23.
//

#ifndef ALGORITHM_ENGINEERING_EM_MERGESORT_H
#define ALGORITHM_ENGINEERING_EM_MERGESORT_H

#include <string>
#include <iostream>

void sort_partitions(std::string &in_file, std::string & out_file, long long blocksize);

void merge_partitions(std::string &in_filename, std::string &out_filename, long long partitionsize, long long blocksize);

void merge_two_parts(std::string &in_filename, std::string &out_filename, long long blocksize, long long partition_size, long long offset);

#endif //ALGORITHM_ENGINEERING_EM_MERGESORT_H
