#pragma once

#include <igraph.h>

#include <set>
#include <string>

void parse_steiner_tree_file(std::string path, igraph_t &graph,
                             igraph_vector_t &weights,
                             std::set<int> &terminals);

void print_graph(igraph_t &graph, igraph_vector_t &weights);

class SteinerTreeResult {
 public:
  std::optional<igraph_t> graph;
  std::optional<igraph_vector_t> weights;
  std::set<int> terminals;
  double total_cost;

  SteinerTreeResult(std::optional<igraph_t>, std::optional<igraph_vector_t>,
                    std::set<int> terminals, double total_cost);

  void write_to_file(std::string path);
  bool validate();
};