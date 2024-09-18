#include "../include/prj/graph.h"

#include <igraph.h>

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>

void parse_steiner_tree_file(std::string path, igraph_t &graph,
                             igraph_vector_t &weights,
                             std::set<int> &terminals) {
  std::regex node_number_pattern("Nodes\\s(\\d+)");
  std::regex edge_number_pattern("Edges\\s(\\d+)");
  std::regex terminal_number_pattern("Terminals\\s(\\d+)");

  std::regex terminal_pattern("T\\s(\\d+)");
  std::regex edge_pattern("E\\s(\\d+)\\s(\\d+)\\s(\\d+\\.?\\d*)");

  std::ifstream graph_file;
  graph_file.open(path);

  if (!graph_file.is_open()) {
    throw std::runtime_error("Could not find Graph File");
  }

  std::smatch match;
  std::string line;

  int n_vertices;
  int n_edges;
  int n_terminals;
  std::vector<std::tuple<int, int, float>> edges;

  while (getline(graph_file, line)) {
    if (std::regex_search(line, match, edge_pattern)) {
      edges.push_back({std::stoi(match.str(1)) - 1, std::stoi(match.str(2)) - 1,
                       std::stof(match.str(3))});
      //            printf("number of edges: %i\n", edges.size());
    } else if (std::regex_search(line, match, terminal_pattern)) {
      terminals.insert(std::stoi(match.str(1)) - 1);
      //            printf("added terminal %i\n", std::stoi(match.str(1)) - 1);
    } else if (std::regex_search(line, match, node_number_pattern)) {
      n_vertices = std::stoi(match.str(1));
      //            printf("Nodes %i\n", n_vertices);
    } else if (std::regex_search(line, match, edge_number_pattern)) {
      n_edges = std::stoi(match.str(1));
      //            printf("Edges %i\n", n_edges);
    } else if (std::regex_search(line, match, terminal_number_pattern)) {
      n_terminals = std::stoi(match.str(1));
      //            printf("Terminals %i\n", n_terminals);
    }
  }

  // initialize empty graph
  igraph_empty(&graph, n_vertices, IGRAPH_UNDIRECTED);
  igraph_vector_init(&weights, n_edges);

  int i = 0;

  // intialize edges and build weight vector
  for (auto &[from, to, w] : edges) {
    //        printf("WWW %i", w);
    igraph_add_edge(&graph, from, to);
    VECTOR(weights)[i++] = w;
  }
}

void print_graph(igraph_t &graph, igraph_vector_t &weights) {
  // iterate over edges and vertices
  // --------------------------------
  igraph_vit_t vit;
  igraph_eit_t eit;
  igraph_vit_create(&graph, igraph_vss_all(), &vit);
  igraph_eit_create(&graph, igraph_ess_all(IGRAPH_EDGEORDER_ID), &eit);

  printf("Nodes: ");
  while (!IGRAPH_VIT_END(vit)) {
    printf(" %" IGRAPH_PRId, IGRAPH_VIT_GET(vit));
    IGRAPH_VIT_NEXT(vit);
  }

  igraph_integer_t efrom, eto;
  int i = 0;
  printf("\nEdges: ");
  while (!IGRAPH_EIT_END(eit)) {
    igraph_integer_t ex = IGRAPH_EIT_GET(eit);
    igraph_edge(&graph, ex, &efrom, &eto);
    printf("%lli: %lli -> %lli with %f\n", ex, efrom, eto,
           VECTOR(weights)[i++]);
    IGRAPH_EIT_NEXT(eit);
  }
}

SteinerTreeResult::SteinerTreeResult(std::optional<igraph_t> graph,
                                     std::optional<igraph_vector_t> weights,
                                     std::set<int> terminals,
                                     double total_cost) {
  this->graph = graph;
  this->weights = weights;
  this->terminals = terminals;
  this->total_cost = total_cost;
}

void SteinerTreeResult::write_to_file(std::string path) {
  std::ofstream file;
  file.open(path);

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for writing");
  }

  if (!this->graph.has_value()) {
    file << "NO RESULT" << std::endl;
    return;
  }

  igraph_t &g = this->graph.value();
  igraph_vector_t &weights = this->weights.value();

  // iterate over edges and vertices
  // --------------------------------
  igraph_vit_t vit;
  igraph_eit_t eit;
  igraph_vit_create(&g, igraph_vss_all(), &vit);
  igraph_eit_create(&g, igraph_ess_all(IGRAPH_EDGEORDER_ID), &eit);

  file << "Nodes " << igraph_vcount(&g) << std::endl;
  file << "Edges " << igraph_ecount(&g) << std::endl;
  file << "Terminals " << this->terminals.size() << std::endl;

  igraph_integer_t efrom, eto;
  int i = 0;
  while (!IGRAPH_EIT_END(eit)) {
    igraph_integer_t ex = IGRAPH_EIT_GET(eit);
    igraph_edge(&g, ex, &efrom, &eto);
    file << "E " << efrom + 1 << " " << eto + 1 << " " << VECTOR(weights)[i++]
         << std::endl;
    IGRAPH_EIT_NEXT(eit);
  }

  for (auto t : terminals) {
    file << "T " << t + 1 << std::endl;
  }
}

bool SteinerTreeResult::validate() {
  if (!this->graph.has_value()) {
    return false;
  }

  igraph_t &g = this->graph.value();
  igraph_vector_t &weights = this->weights.value();
  std::set<int> &terminals = this->terminals;

  igraph_vector_int_t ig_terminals;
  igraph_vector_int_init(&ig_terminals, terminals.size());
  {
    int i = 0;
    for (auto t : terminals) {
      VECTOR(ig_terminals)[i++] = t;
    }
  }

  std::set<int> vertex_set;
  for (int i = 0; i < igraph_vcount(&g); i++) {
    vertex_set.insert(i);
  }

  // check that all terminals are in the graph
  for (auto t : terminals) {
    if (vertex_set.find(t) == vertex_set.end()) {
      printf("Terminal %i not in graph\n", t);
      return false;
    }
  }

  // check that all vertices are connected
  igraph_matrix_t distance_matrix;
  igraph_matrix_init(&distance_matrix, terminals.size(), terminals.size());

  igraph_distances(&g, &distance_matrix, igraph_vss_vector(&ig_terminals),
                   igraph_vss_vector(&ig_terminals), IGRAPH_ALL);

  for (int i = 0; i < igraph_vector_int_size(&ig_terminals); ++i) {
    for (int j = 0; j < igraph_vector_int_size(&ig_terminals); ++j) {
      if (MATRIX(distance_matrix, i, j) == IGRAPH_INFINITY) {
        printf("No path between %i and %i\n", i, j);
        return false;
      }
    }
  }

  return true;
}