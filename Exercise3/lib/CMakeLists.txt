add_library(GraphFunctions graph.cpp)
add_library(2APXFunctions 2apx.cpp)
add_library(BstVcFunctions bst_vc.cpp)
add_library(Utils utils.cpp)

target_link_libraries(GraphFunctions Boost::boost)

target_include_directories(GraphFunctions PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(2APXFunctions PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(BstVcFunctions PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(Utils PUBLIC ${CMAKE_SOURCE_DIR}/include)
