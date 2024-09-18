library(igraph)
library(stringr)
library(magrittr)

#ip <- readLines("~/dev/priv/master/algorithm_engineering/code/exam_steiner/input/app_paper.gr")
ip <- readLines("~/dev/priv/master/algorithm_engineering/code/exam_steiner/build/app_paper_approx.out")

edges <- integer()
weights <- numeric()
terminals <- numeric()

for (l in ip) {
  
  if(stringr::str_detect(l, "^Nodes.*")) {
    n_nodes <- as.integer(stringr::str_extract(l, "\\d+"))
  }
  # else if(stringr::str_detect(l, "^Edges*")) {
  #   n_edges <- as.integer(stringr::str_extract(l, "\\d+"))
  # }
  else if(stringr::str_detect(l, "^Terminals*")) {
    n_terminals <- as.integer(stringr::str_extract(l, "\\d+"))
  }
  else if(stringr::str_detect(l, "^E\\s.*")) {
    print(l)
    es <- stringr::str_match(l, "E ([\\d\\s\\.]+)")[[2]]
    essplit <- as.numeric(stringr::str_split(es, " ")[[1]])
    edges <- c(edges, head(essplit, -1))
    weights <- c(weights, tail(essplit, 1))
    print(tail(essplit, 1))
  } else if(stringr::str_detect(l, "^T\\s.*")) {
    print(l)
    es <- stringr::str_match(l, "T ([\\d]+)")[[2]]
    terminals <- c(terminals, as.numeric(es))
  }
}

g <- make_graph(edges = edges, n = n_nodes, directed = F)

E(g)$weight <- weights

# E(g)$weight <- max(E(g)$weight) - (E(g)$weight - 1)


# V(g)$color <- rep("green", length(V(g)))
V(g)$color <- ifelse(V(g) %in% terminals, "orange", "purple")
V(g)$label <- V(g)

E(g)$color <- rep("blue", length(E(g)))

isolated <- which(degree(g) == 0)
g <- delete.vertices(g, isolated)

plot(g, edge.width=3, edge.label = E(g)$weight, edge.label.cex=1.5, edge.label.color = "red", layout = layout_with_kk)
