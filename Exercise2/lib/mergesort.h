#ifndef ALGORITHM_ENGINEERING_MERGESORT_H
#define ALGORITHM_ENGINEERING_MERGESORT_H

#include <vector>

void merge(std::vector<int> &data, long long left, long long middle, long long right);

void mergeSort(std::vector<int> &data, long long left, long long right);

void ompMerge(std::vector<int> &data, int n_processors, long long left, long long middle, long long right);

void ompMergeSort(std::vector<int> &data, int n_processors, long long left, long long right);

#endif //ALGORITHM_ENGINEERING_MERGESORT_H
