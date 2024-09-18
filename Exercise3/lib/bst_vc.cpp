#include <set>

#include "../include/prj/graph.h"

std::set<int> bst_vc_impl(Graph& g, unsigned int max_k, std::set<int>& vc) {
  int n_vertices = g.vertices.size();

  if (vc.size() > max_k) {
    return std::set<int>();
  }

  int v1;
  int v2;
  bool edge_found = false;

  for (int u = 0; u < n_vertices && !edge_found; u++) {
    if (vc.contains(u)) {
      continue;
    }

    for (int v : g.adjList[u]) {
      if (!vc.contains(v)) {
        edge_found = true;
        v1 = u;
        v2 = v;
        break;
      }
    }
  }

  if (!edge_found) {
    return vc;
  }

  vc.insert(v1);
  std::set<int> res1 = bst_vc_impl(g, max_k, vc);

  if (!res1.empty()) {
    return res1;
  }

  vc.erase(v1);
  vc.insert(v2);

  std::set<int> res2 = bst_vc_impl(g, max_k, vc);

  if (!res2.empty()) {
    return res2;
  }

  vc.erase(v2);
  return {};
}

std::set<int> bounded_search_tree_vertex_cover(Graph& g, unsigned int max_k) {
  std::set<int> vc;

  return bst_vc_impl(g, max_k, vc);
}
