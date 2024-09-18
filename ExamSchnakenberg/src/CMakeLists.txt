add_executable(exact exact_exec.cpp)
add_executable(approximation approximation_exec.cpp)
add_executable(heuristic heuristic_exec.cpp)

target_include_directories(exact PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(approximation PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(heuristic PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(exact igraph GraphFunctions argparse Exact)
target_link_libraries(approximation igraph GraphFunctions argparse Approximation)
target_link_libraries(heuristic igraph GraphFunctions argparse Heuristic)