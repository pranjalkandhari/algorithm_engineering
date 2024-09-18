#include "../include/prj/heuristic.h"

#include <map>

SteinerTreeResult steiner_tree_heuristic(igraph_t& graph,
                                         std::set<int>& terminals,
                                         igraph_vector_t& weights) {
  igraph_vector_int_t terminals_igraph;

  igraph_vector_int_init(&terminals_igraph, terminals.size());
  {
    int i = 0;
    for (int t : terminals) {
      VECTOR(terminals_igraph)[i++] = t;
    }
  }

  igraph_matrix_t distances;
  std::map<std::pair<int, int>, float> distance_map;

  igraph_matrix_init(&distances, terminals.size(), terminals.size());
  igraph_distances_dijkstra(
      &graph, &distances, igraph_vss_vector(&terminals_igraph),
      igraph_vss_vector(&terminals_igraph), &weights, IGRAPH_ALL);

  // save distances as map to avoid mapping of terminal to index in further sets
  {
    int i = 0;
    for (int t1 : terminals) {
      int j = 0;
      for (int t2 : terminals) {
        distance_map[std::make_pair(t1, t2)] = MATRIX(distances, i, j++);
      }
      i++;
    }
  }

  // 1) Start with a subtree T consisting of
  //         one given terminal vertex
  std::set<int> subtree_edges;

  std::set<int> not_included_terminals(terminals);
  std::set<int> included_terminals;

  not_included_terminals.erase(*terminals.begin());
  included_terminals.insert(*terminals.begin());

  // 2) While T does not span all terminals
  //         a) Select a terminal x not in T that is closest
  //         to a vertex in T.
  // b) Add to T the shortest path that connects x with T

  // while not all terminals included
  while (not_included_terminals.size() > 0) {
    // a) Select a terminal x not in T that is closest

    int min_dist_terminal_from, min_dist_terminal_to;
    double min_dist;
    min_dist = std::numeric_limits<double>::max();

    // iterate over all possible not included terminals
    for (auto term_to_add_iter = not_included_terminals.begin();
         term_to_add_iter != not_included_terminals.end(); term_to_add_iter++) {
      for (auto term_to_add_connector_it = included_terminals.begin();
           term_to_add_connector_it != included_terminals.end();
           term_to_add_connector_it++) {
        if (distance_map[std::make_pair(
                *term_to_add_iter, *term_to_add_connector_it)] < min_dist) {
          min_dist = distance_map[std::make_pair(*term_to_add_iter,
                                                 *term_to_add_connector_it)];
          min_dist_terminal_from = *term_to_add_iter;
          min_dist_terminal_to = *term_to_add_connector_it;
        }
      }
    }

    igraph_vector_int_t tt_e;
    igraph_vector_int_init(&tt_e, 0);

    igraph_get_shortest_path_dijkstra(
        &graph,
        NULL,                    // vertices
        &tt_e,                   // edges
        min_dist_terminal_from,  // from
        min_dist_terminal_to,    // to
        &weights,                // weights
        IGRAPH_ALL               // mode, ignored for undirected
    );

    for (int j = 0; j < igraph_vector_int_size(&tt_e); ++j) {
      subtree_edges.insert(VECTOR(tt_e)[j]);
    }

    not_included_terminals.erase(min_dist_terminal_from);
    included_terminals.insert(min_dist_terminal_from);
  }

  igraph_vector_int_t ig_subtree_edges;
  igraph_vector_int_init(&ig_subtree_edges, subtree_edges.size());
  {
    int i = 0;
    for (int e : subtree_edges) {
      VECTOR(ig_subtree_edges)[i++] = e;
    }
  }

  igraph_t ts;
  igraph_subgraph_from_edges(&graph, &ts, igraph_ess_vector(&ig_subtree_edges),
                             false);

  igraph_vector_t ts_weights;
  igraph_vector_init(&ts_weights, subtree_edges.size());

  {
    int i = 0;
    for (int e : subtree_edges) {
      VECTOR(ts_weights)[i++] = VECTOR(weights)[e];
    }
  }

  return SteinerTreeResult(std::optional<igraph_t>(ts),
                           std::optional<igraph_vector_t>(ts_weights),
                           terminals, igraph_vector_sum(&ts_weights));
}