cmake_minimum_required(VERSION 3.26)
project(exercise2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(OPTIMIZED_RELEASE "Release Mode" OFF)

if(OPTIMIZED_RELEASE)
    message("Building in Release Mode")
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else ()
    message("Building in Debug Mode")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

add_subdirectory(lib)

# fetch latest argparse
include(FetchContent)
FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

add_executable(MergeSort mergesort.cpp)
add_executable(OMPMergeSort omp_mergesort.cpp)

target_link_libraries(MergeSort PUBLIC MergeSortFunctions RandomNumberFunctions UtilFunctions argparse)
target_link_libraries(OMPMergeSort PUBLIC MergeSortFunctions RandomNumberFunctions UtilFunctions argparse)

target_include_directories(MergeSort PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/lib"
)

target_include_directories(OMPMergeSort PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/lib"
)