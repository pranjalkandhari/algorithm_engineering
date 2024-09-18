#include "../include/prj/exact.h"

#include <iostream>
#include <limits>
#include <map>
#include <set>

#include "../include/prj/utils.h"

SteinerTreeResult steiner_tree_exact(igraph_t& graph, std::set<int>& terminals,
                                     igraph_vector_t& weights) {
  std::map<std::pair<std::set<int>, int>, double> S;

  // determine q and C
  std::set<int> C(terminals);
  auto c_first_elem_iter = C.begin();
  int q = *c_first_elem_iter;
  C.erase(c_first_elem_iter);

  size_t C_length = C.size();

  // calculate distance between all vertices
  igraph_matrix_t graph_distance_matrix;
  igraph_matrix_init(&graph_distance_matrix, igraph_vcount(&graph),
                     igraph_vcount(&graph));

  igraph_distances_dijkstra(&graph, &graph_distance_matrix, igraph_vss_all(),
                            igraph_vss_all(), &weights, IGRAPH_OUT);

  for (auto t = C.begin(); t != C.end(); t++) {
    for (auto J = 0; J < igraph_vcount(&graph); J++) {
      S[{{*t}, J}] = MATRIX(graph_distance_matrix, *t, J);
    }
  }

  for (int m = 2; m < C_length; ++m) {
    std::vector<std::set<int>> Ds = get_all_subsets(C, m);

    for (std::set<int> D : Ds) {
      for (int I = 0; I < igraph_vcount(&graph); ++I) {
        S[{D, I}] = std::numeric_limits<double>::max();  // actually infinity
      }

      for (int J = 0; J < igraph_vcount(&graph); ++J) {
        double u = std::numeric_limits<double>::max();  // actually infinity
        // for each E such that D[1] in E and E in D
        std::vector<std::set<int>> Es = get_all_subsets_with_size(D, 1, m - 1);
        for (std::set<int> E : Es) {
          std::set<int> D_minus_E;
          std::set_difference(D.begin(), D.end(), E.begin(), E.end(),
                              std::inserter(D_minus_E, D_minus_E.begin()));

          u = std::min(u, S.at({E, J}) + S.at({D_minus_E, J}));
        }

        for (int I = 0; I < igraph_vcount(&graph); ++I) {
          double tttt =
              std::min(S.at({D, I}), MATRIX(graph_distance_matrix, I, J) + u);
          S[{D, I}] = tttt;
        }
      }
    }
  }

  double v = std::numeric_limits<double>::max();  // actually infinity

  for (int J = 0; J < igraph_vcount(&graph); ++J) {
    double u = std::numeric_limits<double>::max();  // actually infinity
    std::vector<std::set<int>> Es =
        get_all_subsets_with_size(C, 1, C_length - 1);
    for (std::set<int> E : Es) {
      std::set<int> C_minus_E;
      std::set_difference(C.begin(), C.end(), E.begin(), E.end(),
                          std::inserter(C_minus_E, C_minus_E.begin()));

      u = std::min(u, S.at({E, J}) + S.at({C_minus_E, J}));
      v = std::min(v, MATRIX(graph_distance_matrix, q, J) + u);
    }
  }

  return SteinerTreeResult(std::nullopt, std::nullopt, terminals, v);
}