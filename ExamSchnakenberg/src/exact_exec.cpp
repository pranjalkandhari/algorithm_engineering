#include <igraph.h>

#include <argparse/argparse.hpp>
#include <chrono>

#include "../include/prj/exact.h"

int main(int argc, char** argv) {
  argparse::ArgumentParser program(argv[0]);

  program.add_argument("-gr", "--graph-input")
      .required()
      .help("Path for the graph file input");

  program.add_argument("-o", "--output").required().help("Path for the output");

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  igraph_t graph;
  igraph_vector_t weights;
  std::set<int> terminals;

  std::string graph_path = program.get("-gr");
  std::string output_path = program.get("-o");

  auto start_parse = std::chrono::high_resolution_clock::now();
  parse_steiner_tree_file(graph_path, graph, weights, terminals);
  auto stop_parse = std::chrono::high_resolution_clock::now();

  auto duration_parse = std::chrono::duration_cast<std::chrono::microseconds>(
      stop_parse - start_parse);

  printf("graph_nodes:%lli\n", igraph_vcount(&graph));
  printf("graph_edges:%lli\n", igraph_ecount(&graph));
  printf("graph_weight:%f\n", igraph_vector_sum(&weights));
  printf("terminals:%zu\n", terminals.size());
  printf("parse_time:%lli\n", duration_parse.count());
  fflush(stdout);

  auto start_steiner = std::chrono::high_resolution_clock::now();
  SteinerTreeResult r = steiner_tree_exact(graph, terminals, weights);
  auto stop_steiner = std::chrono::high_resolution_clock::now();

  auto duration_steiner = std::chrono::duration_cast<std::chrono::microseconds>(
      stop_steiner - start_steiner);

  printf("steiner_weight:%f\n", r.total_cost);
  printf("steiner_time:%lli\n", duration_steiner.count());
  fflush(stdout);

  r.write_to_file(output_path);

  return 0;
}