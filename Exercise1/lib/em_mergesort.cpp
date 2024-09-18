//
// Created by Nils Schnakenberg on 07.11.23.
//

#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <filesystem>

#include "io_utils.h"
#include "em_mergesort.h"
#include "mergesort.h"
#include <cmath>

// Function to sort partitions of data stored in 'in_file' and write the sorted partitions to 'out_file'
void sort_partitions(std::string &in_file, std::string & out_file, long long blocksize) {
    auto file_size = get_file_size(in_file);

    // Calculate the number of batches (partitions) based on blocksize
    int batches = std::ceil((double)file_size / blocksize);

    // Iterate through each batch (partition)
    for (int i = 0; i < batches; i++) {

        std::cout << "Sorting partition " << i+1 << " of " << batches << std::endl;

        // Calculate the offset for the current batch
        long long offset = i * blocksize;
        // Read the block of data from 'in_file' at the calculated offset and of size 'blocksize'  
        std::vector<int> data = read_block(in_file,offset, blocksize);

        // Determine the rightmost index for sorting (minimum of blocksize/4 and data size - 1)
        long long r = std::min((long long)(blocksize / 4) - 1, (long long)data.size()-1);

        // Apply the merge sort algorithm to the current block of data
        mergeSort(data, 0, r);

        // Write the sorted data back to 'out_file' at the calculated offset
        write_integers_at_offset(out_file, data, offset);
    }
}

// Function to merge sorted partitions in the input file and write the merged partitions to the output file
void merge_partitions(std::string &in_filename, std::string &out_filename, long long partitionsize, long long blocksize) {

    long long in_filesize = get_file_size(in_filename);
    long long offset;

    long long number_of_rounds = 0;

    std::cout << "in_filename: " << in_filename << std::endl;
    std::cout << "in_filesize: " << in_filesize << std::endl;
    std::cout << "partitionsize: " << partitionsize << std::endl;


    // Continue merging until the partition size is greater than the input file size
    while (partitionsize < in_filesize) {

        number_of_rounds++;

        std::cout << "Merging fraction: " << ((double)partitionsize)/in_filesize << std::endl;

        offset = 0;
        // Iterate through the input file, merging two parts at a time
        while (offset < in_filesize) {
            std::cout << "Merging running offset: " << ((double)offset)/in_filesize << std::endl;
            merge_two_parts(in_filename, out_filename, blocksize, partitionsize, offset);
            offset += 2 * partitionsize;
        }
        partitionsize *= 2;
        std::swap(in_filename, out_filename);

        // Clear out the output file for the next iteration
        std::ofstream clear_out_file(out_filename, std::ios::out | std::ios::trunc);
        clear_out_file.close();
    }

    std::cout << "Number of Rounds: " << number_of_rounds << std::endl;
}


// Function to merge two sorted partitions in the input file and write the merged partition to the output file
void merge_two_parts(std::string &in_filename, std::string &out_filename, long long blocksize, long long partition_size, long long offset) {
    // Open input and output files
    std::fstream in_file(in_filename, std::ios::binary | std::ios::in);
    std::fstream out_file(out_filename, std::ios::binary | std::ios::app);

    // Get the size of the input file
    auto in_filesize = get_file_size(in_filename);

    // Calculate the number of elements and blocks in a partition based on block size
    long long n_in_block = blocksize / sizeof(int); // number of elements in a block
    long long n_elements = partition_size / sizeof(int);
    long long n_blocks_in_partition = n_elements / n_in_block;

    int block_idx_x = 0, block_idx_y = 0; // index of input blocks in memory
    int xi = 0, yi = 0; // running index in input blocks

    long long out_idx = 0; // running index to output file to write to correct location

    long long input_offset_x, input_offset_y; // running indices pointing to start of block x and y in input file

    input_offset_x = offset + blocksize * block_idx_x;
    input_offset_y = offset + partition_size + blocksize * block_idx_y;

    // read block x from in_file starting at input_offset_x with blocksize
    // read block y from in_file starting at input_offset_y with blocksize
    std::vector<int> x = read_block(in_filename, input_offset_x, blocksize);
    std::vector<int> y = read_block(in_filename, input_offset_y, blocksize);

    std::vector<int> result;
    result.reserve(blocksize);

    bool last_block_reached = false;

    while(!last_block_reached) {
        // Walk left and right vector and write into output buffer
        while (xi < x.size() && yi < y.size()) {
            if(x[xi]< y[yi]) {
                result.push_back(x[xi]);
                xi++;
            } else {
                result.push_back(y[yi]);
                yi++;
            }
            // If the result buffer is full, write it to the output file and clear the buffer
            if(result.size() == n_in_block) {
                write_integers_at_offset(out_filename, result, offset + out_idx++ * blocksize);
                result.clear();
            }
        }
        // If the end of block x is reached, read the next block
        if(xi == x.size()) {
            block_idx_x++;
            input_offset_x = offset + blocksize * block_idx_x;
            if(block_idx_x == n_blocks_in_partition) {
                last_block_reached = true;
            } else {
                x = read_block(in_filename, input_offset_x, blocksize);
                xi = 0;
            }
        }
        // If the end of block y is reached, read the next block
        if(yi == y.size()) {
            block_idx_y++;
            input_offset_y = offset + partition_size + blocksize * block_idx_y;
            if(block_idx_y == n_blocks_in_partition) {
                last_block_reached = true;
            } else {
                y = read_block(in_filename, input_offset_y, blocksize);
                yi = 0;
            }
        }
    }
    // If the end of block x is reached, merge the remaining elements of block y
    if(block_idx_x == n_blocks_in_partition) {
        while (block_idx_y < n_blocks_in_partition) {
            while (yi < n_in_block && yi < y.size()) {
                result.push_back(y[yi]);
                yi++;
                if(result.size() == n_in_block) {
                    write_integers_at_offset(out_filename, result, offset + out_idx++ * blocksize);
                    result.clear();
                }
            }
            block_idx_y++;
            input_offset_y = offset + partition_size + blocksize * block_idx_y;
            y = read_block(in_filename, input_offset_y, blocksize);
            yi = 0;
        }
    }
    // If the end of block y is reached, merge the remaining elements of block x
    if(block_idx_y == n_blocks_in_partition) {
        while(block_idx_x < n_blocks_in_partition) {
            while (xi < n_in_block && xi < x.size()) {
                result.push_back(x[xi]);
                xi++;
                if(result.size() == n_in_block) {
                    write_integers_at_offset(out_filename, result, offset + out_idx++ * blocksize);
                    result.clear();
                }
            }
            block_idx_x++;
            input_offset_x = offset + blocksize * block_idx_x;
            x = read_block(in_filename, input_offset_x, blocksize);
            xi = 0;
        }
    }
    // Write any remaining elements in the result buffer to the output file
    write_integers_at_offset(out_filename, result, offset + out_idx * blocksize);
}