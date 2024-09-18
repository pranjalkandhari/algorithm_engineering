//
// Created by Nils Schnakenberg on 03.11.23.
//

#include <iostream>
#include <vector>

#include "quicksort.h"

void quickSort(std::vector<int> &data, int left, int right) {
    int i = left, j = right;
    int a;

    int pivot = data[(left+right)/2];

    // std::cout << "Before sorting from " << left << " to " << right << " with pivot " << pivot << std::endl;
//    for (int k = left; k <= right; k++) {
//        std::cout << data[k] << " ";
//    }
//    std::cout << std::endl;

    while (i <= j) {
        while (data[i] < pivot) {
            i++;
        }
        while (data[j] > pivot) {
            j--;
        }
        if(i <= j) {
            a = data[i];
            data[i] = data[j];
            data[j] = a;
            i++;
            j--;
        }
    }

    // std::cout << "After sorting from " << left << " to " << right << std::endl;

//    for (int k = left; k <= right; k++) {
//        std::cout << data[k] << " ";
//    }
//    std::cout << std::endl;

    if (left < j)
        quickSort(data, left, j);
    if (i < right)
        quickSort(data, i, right);

}
