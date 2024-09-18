#include <iostream>
#include <list>
#include <set>
#include <vector>

#ifndef EXERCISE3_GRAPH_H
#define EXERCISE3_GRAPH_H

class Graph {
 public:
  std::set<int> vertices;
  std::vector<std::set<int>> adjList;

  Graph(unsigned int n_vertices);

  Graph(const Graph& rhs);

  void add_vertice(int w);

  void add_edge(int u, int v);

  void remove_edge(int u, int v);

  void remove_edges_by_vertice(int w);

  std::set<int> get_vertice_edge_targets(int v);

  std::tuple<int, int> get_first_edge();

  bool has_edges();

  bool is_vertex_cover(std::set<int> cover);

  void print();
};

std::tuple<int, int> parse_line(std::string l);

Graph parse_file(std::string path);

#endif  // EXERCISE3_GRAPH_H
