#include <iostream>
#include <vector>

using namespace std;

// Merge function to merge two subarrays of 'data' array.
// The first subarray is data[left...middle]
// The second subarray is data[middle+1...right].
void merge(std::vector<int> &data, long long left, long long middle, long long right) {

  // Calculate the sizes of the two subarrays.
  long long leftVecSize = middle - left + 1;
  long long rightVecSize = right - middle;

  // Create temporary vectors to hold the left and right subarrays
  std::vector<int> leftVec(leftVecSize);
  std::vector<int> rightVec(rightVecSize);

  // Copy data to temporary arrays leftVec[] and rightVec[]
  for (long long i = 0; i < leftVecSize; i++)
  {
      leftVec[i] = data[left + i];
  }
  for (long long i = 0; i < rightVecSize; i++)
  {
      rightVec[i] = data[middle + 1 + i];
  }

  // Merge the two subarrays back into the original array
  long long i = 0; // left array index
  long long j = 0; // right array index
  long long k = left; // orig array index


  while (i < leftVecSize && j < rightVecSize)
  {
    if(leftVec[i] < rightVec[j]) {
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
  while (i < leftVecSize)
  {
      data[k] = leftVec[i];
    i++;
    k++;
  }

  // Copy remaining right elements
  while (j < rightVecSize)
  {
      data[k] = rightVec[j];
    j++;
    k++;
  }

}

// Merge Sort function to sort the elements of 'data' array in the range from index 'left' to 'right'
void mergeSort(std::vector<int> &data, long long left, long long right) {
  // Check if the array has more than one element.
  if(left >= right) {

//    if(left > right) {
//      cout << "unexpected" << endl;
//    }
//    else {
//      cout << "value at: " << left << " = " << data[left] << endl;
//    }

    return;
  }

  // Calculate the middle index
  long long middle = (left + right) / 2;

  // Recursive calls for the left and right subarrays
  mergeSort(data, left, middle);
  mergeSort(data, middle + 1, right);

  // Merge the sorted subarrays
  merge(data, left, middle, right);
}
