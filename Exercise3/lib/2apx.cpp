#include "../include/prj/2apx.h"

#include "../include/prj/graph.h"

std::set<int> twoApx(Graph &g) {
  Graph g_work(g);
  std::set<int> c;

  while (g_work.has_edges()) {
    auto e = g_work.get_first_edge();
    auto v1 = std::get<0>(e);
    auto v2 = std::get<1>(e);

    g_work.remove_edges_by_vertice(v1);
    g_work.remove_edges_by_vertice(v2);
    c.insert(v1);
    c.insert(v2);
  }

  return c;
}

std::set<int> twoApx2(Graph &g) {
  int n_vertices = g.vertices.size();
  std::set<int> c;

  std::vector<bool> visited(n_vertices, false);

  for (int u = 0; u < n_vertices; u++) {
    for (int v : g.adjList[u]) {
      if (!visited[u] && !visited[v]) {
        c.insert(u);
        c.insert(v);
        visited[u] = visited[v] = true;
      }
    }
  }
  return c;
}