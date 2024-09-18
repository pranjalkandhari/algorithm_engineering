enable_testing()

add_executable(
        test_graph_datastructure
        test_graph_datastructure.cpp
)

target_link_libraries(
        test_graph_datastructure
        GTest::gtest_main
        GraphFunctions
)

include(GoogleTest)
gtest_discover_tests(test_graph_datastructure)


