add_library(MergeSortFunctions mergesort.cpp)

add_library(QuickSortFunctions quicksort.cpp)

add_library(RandomNumberFunctions random_numbers.cpp)

add_library(UtilFunctions utils.cpp)


find_package(OpenMP)
target_link_libraries(MergeSortFunctions PUBLIC OpenMP::OpenMP_CXX)