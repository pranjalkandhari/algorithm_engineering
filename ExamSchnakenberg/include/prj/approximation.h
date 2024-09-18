#pragma once

#include <igraph.h>

#include "graph.h"

SteinerTreeResult steiner_tree_approximation(igraph_t &graph,
                                             std::set<int> &terminals,
                                             igraph_vector_t &weights,
                                             int num_threads);
