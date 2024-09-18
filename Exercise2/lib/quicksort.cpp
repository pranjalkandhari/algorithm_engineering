#include <iostream>
#include <vector>

#include "quicksort.h"

void quickSort(std::vector<int> &data, int left, int right) {
    int i = left, j = right;
    int a;

    int pivot = data[(left + right) / 2];

    while (i <= j) {
        while (data[i] < pivot) {
            i++;
        }
        while (data[j] > pivot) {
            j--;
        }
        if (i <= j) {
            a = data[i];
            data[i] = data[j];
            data[j] = a;
            i++;
            j--;
        }
    }

    if (left < j)
        quickSort(data, left, j);
    if (i < right)
        quickSort(data, i, right);

}
