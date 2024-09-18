#include "../include/prj/graph.h"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <stdexcept>

std::tuple<int, int> parse_line(std::string l) {
  std::vector<std::string> ll;
  boost::split(ll, l, boost::is_any_of(" \t"));

  return std::make_tuple(std::stoi(ll[0]), std::stoi(ll[1]));
}

std::tuple<int, int> parse_header(std::string l) {
  std::vector<std::string> ll;
  boost::split(ll, l, boost::is_any_of(" \t"));

  return std::make_tuple(std::stoi(ll[2]), std::stoi(ll[3]));
}

Graph parse_file(std::string path) {
  std::ifstream graph_file;
  graph_file.open(path);

  if (!graph_file.is_open()) {
    throw std::runtime_error("Could not find Graph File");
  }

  std::string line;

  getline(graph_file, line);
  auto header = parse_header(line);

  Graph g(std::get<0>(header));

  while (getline(graph_file, line)) {
    std::tuple<int, int> x = parse_line(line);
    g.add_edge(std::get<0>(x), std::get<1>(x));
  }

  return g;
}

Graph::Graph(unsigned int n_vertices) {
  vertices = std::set<int>();
  for (int i = 0; i < n_vertices; i++) {
    vertices.insert(i);
  }
  adjList = std::vector<std::set<int>>(n_vertices + 1, std::set<int>());
}

Graph::Graph(const Graph& rhs) {
  vertices = std::set<int>(rhs.vertices);
  adjList = std::vector<std::set<int>>(rhs.adjList);
}

void Graph::add_vertice(int w) {
  if (vertices.contains(w)) {
    throw std::runtime_error("Graph already contains vertice");
  }
  vertices.insert(w);
}

void Graph::add_edge(int u, int v) {
  adjList[u].insert(v);
  adjList[v].insert(u);
}

void Graph::remove_edge(int u, int v) {
  adjList[u].erase(v);
  adjList[v].erase(u);
}

std::set<int> Graph::get_vertice_edge_targets(int v) { return adjList[v]; }

std::tuple<int, int> Graph::get_first_edge() {
  int u, v;

  for (int i = 0; i < adjList.size(); i++) {
    if (!adjList[i].empty()) {
      u = i;
      v = *(adjList[i].begin());
      i = adjList.size();
    }
  }
  return std::make_tuple(u, v);
}

bool Graph::has_edges() {
  for (std::set<int> adj : adjList) {
    if (!adj.empty()) {
      return true;
    }
  }
  return false;
}

void Graph::remove_edges_by_vertice(int w) {
  std::set<int> w_edges = adjList[w];
  for (int w_e : w_edges) {
    adjList[w_e].erase(w);
  }
  adjList[w].clear();
}

bool Graph::is_vertex_cover(std::set<int> cover) {
  Graph gcopy(*this);

  for (int c : cover) {
    gcopy.remove_edges_by_vertice(c);
  }

  return gcopy.has_edges();
}

void Graph::print() {
  std::cout << "========= "
            << "Vertices"
            << " =========" << std::endl;
  for (int v : vertices) {
    std::cout << v << std::endl;
  }
  std::cout << "========= "
            << "Edges"
            << " =========" << std::endl;
  for (int i = 0; i < adjList.size(); i++) {
    for (auto j : adjList[i]) {
      std::cout << i - 1 << " -> " << j - 1 << std::endl;
    }
  }
}