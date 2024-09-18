add_executable(2APX 2apx.cpp)
add_executable(BST bst.cpp)

target_include_directories(2APX PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(BST PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(2APX GraphFunctions 2APXFunctions argparse Utils)
target_link_libraries(BST GraphFunctions BstVcFunctions argparse Utils)
