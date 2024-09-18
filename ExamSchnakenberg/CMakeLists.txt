cmake_minimum_required(VERSION 3.27)
project(exam_steiner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(OPTIMIZED_RELEASE)
    message("Building in Release Mode")
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else ()
    message("Building in Debug Mode")
    set(CMAKE_BUILD_TYPE "Debug")
    add_definitions(-DDEBUG_MODE=1)
endif ()

include(FetchContent)

FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
        GIT_TAG 1b3abd9b929c8d4aed08632824374cf8a55f5a74
)
FetchContent_MakeAvailable(argparse)

FetchContent_Declare(
        igraph
        GIT_REPOSITORY https://github.com/igraph/igraph.git
        GIT_TAG 19c4a25027d54b6a17d2afab63cae561500bee5a
)
FetchContent_MakeAvailable(igraph)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.50 COMPONENTS REQUIRED)
message(STATUS "Boost version: ${Boost_VERSION}")
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenMP)

add_subdirectory(src)
add_subdirectory(lib)