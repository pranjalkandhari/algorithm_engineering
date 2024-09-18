#include "../include/prj/approximation.h"

#include <omp.h>

#include <map>
#include <set>

std::map<std::pair<int, int>, igraph_vector_int_t> calculate_shortest_paths(
    igraph_t &graph, std::set<int> &terminals,
    igraph_vector_int_t &ig_terminals, igraph_vector_t &weights) {
  std::map<std::pair<int, int>, igraph_vector_int_t> shortest_paths;

  for (int i = 0; i < terminals.size(); ++i) {
    for (int j = i + 1; j < terminals.size(); ++j) {
      igraph_vector_int_t st_edges;
      igraph_vector_int_init(&st_edges, 0);
      igraph_get_shortest_path_dijkstra(&graph,
                                        NULL,                     // vertices
                                        &st_edges,                // edges
                                        VECTOR(ig_terminals)[i],  // from
                                        VECTOR(ig_terminals)[j],  // to
                                        &weights, IGRAPH_ALL);
      shortest_paths.insert(
          {{VECTOR(ig_terminals)[i], VECTOR(ig_terminals)[j]}, st_edges});
    }
  }
  return shortest_paths;
}

std::map<std::pair<int, int>, igraph_vector_int_t>
calculate_shortest_paths_parallel(igraph_t &graph, std::set<int> &terminals,
                                  igraph_vector_int_t ig_terminals,
                                  igraph_vector_t &weights, int num_threads) {
  omp_set_num_threads(num_threads);

  std::map<std::pair<int, int>, igraph_vector_int_t> shortest_paths;

#pragma omp parallel for collapse(2)
  for (int i = 0; i < terminals.size(); ++i) {
    for (int j = i + 1; j < terminals.size(); ++j) {
      igraph_vector_int_t st_edges;
      igraph_vector_int_init(&st_edges, 0);
      igraph_get_shortest_path_dijkstra(&graph,
                                        NULL,                     // vertices
                                        &st_edges,                // edges
                                        VECTOR(ig_terminals)[i],  // from
                                        VECTOR(ig_terminals)[j],  // to
                                        &weights, IGRAPH_ALL);
#pragma omp critical
      shortest_paths.insert(
          {{VECTOR(ig_terminals)[i], VECTOR(ig_terminals)[j]}, st_edges});
    }
  }

  return shortest_paths;
}

SteinerTreeResult steiner_tree_approximation(igraph_t &graph,
                                             std::set<int> &terminals,
                                             igraph_vector_t &weights,
                                             int num_threads) {
  // convert terminals to igraph vector
  igraph_vector_int_t ig_terminals;
  igraph_vector_int_init(&ig_terminals, terminals.size());
  {
    int i = 0;
    for (auto it = terminals.begin(); it != terminals.end(); it++) {
      VECTOR(ig_terminals)[i++] = *it;
    }
  }

  // Construct the complete undirected distance graph G, =(V,E,d) from G

  // calculate distance between all pairs of terminals
  igraph_matrix_t graph_distance_matrix;
  igraph_matrix_init(&graph_distance_matrix, terminals.size(),
                     terminals.size());
  igraph_distances_dijkstra(
      &graph, &graph_distance_matrix, igraph_vss_vector(&ig_terminals),
      igraph_vss_vector(&ig_terminals), &weights, IGRAPH_OUT);

  // calculate shortest paths between all pairs of terminals
  std::map<std::pair<int, int>, igraph_vector_int_t> shortest_paths =
      num_threads > 1
          ? calculate_shortest_paths_parallel(graph, terminals, ig_terminals,
                                              weights, num_threads)
          : calculate_shortest_paths(graph, terminals, ig_terminals, weights);

  // initialize g1 (distance graph between terminals)
  igraph_t g1;
  igraph_empty(&g1, terminals.size(), IGRAPH_UNDIRECTED);

  igraph_vector_t g1_weights;
  igraph_vector_init(&g1_weights, 0);

  for (int i = 0; i < terminals.size(); i++) {
    for (int j = i + 1; j < terminals.size(); j++) {
      igraph_add_edge(&g1, i, j);
      igraph_vector_push_back(&g1_weights, MATRIX(graph_distance_matrix, i, j));
    }
  }

  // Find the minimal spanning tree of G1.

  // init data for minimum spanning tree
  igraph_t g1_spanning_tree;
  igraph_vector_int_t g1_spanning_tree_edges;
  igraph_vector_t g1_spanning_tree_weights;

  igraph_vector_int_init(&g1_spanning_tree_edges, 0);
  igraph_vector_init(&g1_spanning_tree_weights, 0);

  igraph_minimum_spanning_tree(&g1,                      // input graph
                               &g1_spanning_tree_edges,  // result edges
                               &g1_weights               // weights
  );

  igraph_subgraph_from_edges(
      &g1,                                         // input graph
      &g1_spanning_tree,                           // result graph
      igraph_ess_vector(&g1_spanning_tree_edges),  // edge selector
      false                                        // delete vertices
  );

  // get weights of minimum spanning tree
  for (int i = 0; i < igraph_vector_int_size(&g1_spanning_tree_edges); ++i) {
    igraph_vector_push_back(&g1_spanning_tree_weights, VECTOR(g1_weights)[i]);
  }

  //  Expand the minimal spanning tree to a graph Gs by adding the
  //  shortest paths of all edges in the minimal spanning tree.

  // init data for expanded minimum spanning tree
  igraph_t gs;
  igraph_vector_t gs_weights;
  igraph_empty(&gs, igraph_vcount(&graph), IGRAPH_UNDIRECTED);
  igraph_vector_init(&gs_weights, 0);

  // iterate over edges of minimum spanning tree and add expanded edges to gs
  igraph_eit_t g1_spanning_tree_eit;
  igraph_eit_create(&g1_spanning_tree, igraph_ess_all(IGRAPH_EDGEORDER_ID),
                    &g1_spanning_tree_eit);

  while (!IGRAPH_EIT_END(g1_spanning_tree_eit)) {
    igraph_integer_t eid, efrom, eto;
    eid = IGRAPH_EIT_GET(g1_spanning_tree_eit);
    igraph_edge(&g1_spanning_tree, eid, &efrom, &eto);

    if (efrom > eto) {
      std::swap(efrom, eto);
    }

    igraph_vector_int_t e_sp = shortest_paths.at(
        {VECTOR(ig_terminals)[efrom], VECTOR(ig_terminals)[eto]});

    for (int i = 0; i < igraph_vector_int_size(&e_sp); ++i) {
      // add edge to gs
      igraph_integer_t e_sp_id, e_sp_from, e_sp_to, e_dup_id;
      e_sp_id = VECTOR(e_sp)[i];
      igraph_edge(&graph, e_sp_id, &e_sp_from, &e_sp_to);
      igraph_get_eid(&gs, &e_dup_id, e_sp_from, e_sp_to, 0, 0);

      if (e_dup_id == -1) {
        igraph_add_edge(&gs, e_sp_from, e_sp_to);
        // add weight to gs_weights
        igraph_vector_push_back(&gs_weights, VECTOR(weights)[VECTOR(e_sp)[i]]);
      }
    }
    IGRAPH_EIT_NEXT(g1_spanning_tree_eit);
  }

  // Find the minimal spanning tree, Ts, of Gs.

  // init data for minimum spanning tree Ts of expanded minimum spanning tree Gs
  igraph_t ts;
  igraph_vector_int_t ts_edges;
  igraph_vector_t ts_weights;

  igraph_vector_int_init(&ts_edges, 0);
  igraph_vector_init(&ts_weights, 0);

  igraph_minimum_spanning_tree(&gs,        // input graph
                               &ts_edges,  // result edges
                               &gs_weights);

  igraph_subgraph_from_edges(&gs,                           // input graph
                             &ts,                           // result graph
                             igraph_ess_vector(&ts_edges),  // edge selector
                             false                          // delete vertices
  );

  for (int i = 0; i < igraph_vector_int_size(&ts_edges); ++i) {
    igraph_vector_push_back(&ts_weights,
                            VECTOR(gs_weights)[VECTOR(ts_edges)[i]]);
  }

  // prune Ts by removing non-terminal leafs

  igraph_vector_int_t ts_node_degrees;
  igraph_vector_int_init(&ts_node_degrees, igraph_vcount(&ts));
  bool non_terminal_leafs = true;

  // iteratively remove non-terminal leafs
  // until no more non-terminal leafs are found
  while (non_terminal_leafs) {
    non_terminal_leafs = false;

    // calculate degree for every vertex
    igraph_degree(&ts,               // input graph
                  &ts_node_degrees,  // result degrees
                  igraph_vss_all(),  // vertices
                  IGRAPH_ALL,        // mode (ignored for undirected)
                  true               // loops
    );

    for (int i = 0; i < igraph_vector_int_size(&ts_node_degrees); ++i) {
      // if vertex is a non-terminal leaf
      if (VECTOR(ts_node_degrees)[i] == 1 &&
          terminals.find(i) == terminals.end()) {
        // mark loop for another iteration
        non_terminal_leafs = true;

        // find and remove incident edge of non-terminal leaf
        igraph_vector_int_t incident_edges;
        igraph_vector_int_init(&incident_edges, 0);
        igraph_incident(&ts,              // input graph
                        &incident_edges,  // result edges
                        i,                // vertex
                        IGRAPH_ALL        // mode
        );

        igraph_delete_edges(&ts,                                // input graph
                            igraph_ess_vector(&incident_edges)  // edges
        );

        // remove weight of removed edge
        for (int i = 0; i < igraph_vector_int_size(&incident_edges); ++i) {
          igraph_vector_remove(&ts_weights, VECTOR(incident_edges)[i]);
        }
      }
    }
  }

  return SteinerTreeResult(std::optional<igraph_t>(ts),
                           std::optional<igraph_vector_t>(ts_weights),
                           terminals, igraph_vector_sum(&ts_weights));
}