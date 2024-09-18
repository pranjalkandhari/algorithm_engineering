#include <iostream>
#include <vector>
#include <thread>

#include <omp.h>
#include "utils.h"

using namespace std;

int binary_search_index(std::vector<int> &v, int f) {
    auto it = std::lower_bound(v.begin(), v.end(), f);
    return std::distance(v.begin(), it);
}

void merge(std::vector<int> &data, long long left, long long middle, long long right) {

    long long leftVecSize = middle - left + 1;
    long long rightVecSize = right - middle;

    // Create temporary vectors to hold the left and right subarrays
    std::vector<int> leftVec(leftVecSize);
    std::vector<int> rightVec(rightVecSize);

    // Copy data to temporary arrays leftVec[] and rightVec[]
    for (long long i = 0; i < leftVecSize; i++) {
        leftVec[i] = data[left + i];
    }
    for (long long i = 0; i < rightVecSize; i++) {
        rightVec[i] = data[middle + 1 + i];
    }

    long long i = 0; // left array index
    long long j = 0; // right array index
    long long k = left; // orig array index


    while (i < leftVecSize && j < rightVecSize) {
        if (leftVec[i] < rightVec[j]) {
            data[k] = leftVec[i];
            i++;
        } else {
            data[k] = rightVec[j];
            j++;
        }
        k++;
    }

    // left or right array walked completely

    // Copy remaining left elements
    while (i < leftVecSize) {
        data[k] = leftVec[i];
        i++;
        k++;
    }

    // Copy remaining right elements
    while (j < rightVecSize) {
        data[k] = rightVec[j];
        j++;
        k++;
    }

}

void mergeSort(std::vector<int> &data, long long left, long long right) {

    if (left >= right) return;

    long long middle = (left + right) / 2;

    // recursive partitioning phase
    mergeSort(data, left, middle);
    mergeSort(data, middle + 1, right);

    // merging phase
    merge(data, left, middle, right);
}

void ompMerge(std::vector<int> &data, int n_processors, long long left, long long middle, long long right) {
    long long leftVecSize = middle - left + 1;
    long long rightVecSize = right - middle;

    // Create temporary vectors to hold the left and right subarrays
    std::vector<int> leftVec(leftVecSize);
    std::vector<int> rightVec(rightVecSize);

    omp_set_num_threads(n_processors);

    // Copy data to temporary arrays leftVec[] and rightVec[]
    //#pragma omp parallel for
//#pragma omp simd
    for (long long i = 0; i < leftVecSize; i++) {
        leftVec[i] = data[left + i];
    }
    //#pragma omp parallel for
//#pragma omp simd
    for (long long i = 0; i < rightVecSize; i++) {
        rightVec[i] = data[middle + 1 + i];
    }


#pragma omp parallel shared(leftVec, rightVec, leftVecSize, rightVecSize, data, left) default(none)
    {
        // for each element in left
        // search for (theoretical) position in right 'j'
        // assign data[i+j] = element
        #pragma omp for nowait
        for (long long i = 0; i < leftVecSize; i++) {
            int element = leftVec[i];
            int j = binary_search_index(rightVec, element);
            data[left + i + j] = element;
        }

        // for each element in right
        // search for (theoretical) position in left 'j'
        // assign data[i+j] = element
        #pragma omp for
        for (long long i = 0; i < rightVecSize; i++) {
            int element = rightVec[i];
            int j = binary_search_index(leftVec, element);
            data[left + i + j] = element;
        }
    };

}

void ompMergeSort(std::vector<int> &data, int n_processors, long long left, long long right) {

    if (left >= right) return;

    long long middle = (left + right) / 2;

    // recursive partitioning phase
    ompMergeSort(data, n_processors, left, middle);
    ompMergeSort(data, n_processors, middle + 1, right);

    // merging phase
    ompMerge(data, n_processors, left, middle, right);
}