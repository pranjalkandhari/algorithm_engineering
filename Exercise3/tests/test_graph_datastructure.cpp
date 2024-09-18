#include <gtest/gtest.h>

#include "../include/prj/graph.h"

TEST(GraphCreation, EmptyGraph) {
  Graph g(0);

  EXPECT_EQ(g.adjList.size(), 0);
  EXPECT_EQ(g.vertices.size(), 0);
}

TEST(GraphCreation, PrefilledGraph) {
  Graph g(100);

  EXPECT_EQ(g.adjList.size(), 100);
  EXPECT_EQ(g.vertices.size(), 100);
}

// Demonstrate some basic assertions.
TEST(GraphModification, AddVertices) {
  Graph g(100);

  EXPECT_EQ(g.vertices.size(), 100);

  for (int i = 0; i < 100; ++i) {
    g.add_vertice(i + 100);
  }

  EXPECT_EQ(g.vertices.size(), 200);
}