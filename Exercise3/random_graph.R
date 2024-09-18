file_path <- "~/dev/priv/master/algorithm_engineering/code/exercise3/input/test"
max_vert <- 50
n_graphs <- 200

generate_graph <- function(file_path, idx, max_vert) {
  n_vertices <- ceiling(runif(1, 10, max_vert))
  n_edges <- ceiling(runif(1,1,(n_vertices * (n_vertices-1)) / 3))

  edges <- character()
  n_edges_orig <- n_edges

  while(n_edges > 0) {
    edge <- ceiling(runif(2, 1, n_vertices))
    edge_str <- glue::glue("{x} {y}", x = edge[1], y = edge[2])
    edge_str_rev <- glue::glue("{y} {x}", x = edge[1], y = edge[2])
    if(edge[1] != edge[2] && !(edge_str %in% edges) && !(edge_str_rev %in% edges)) {
      edges <- c(edges, edge_str)
      n_edges <- n_edges - 1
    }
  }

  file_name <- paste0(file_path, "/test_", idx, ".gr")
  write(glue::glue("p td {n_vertices} {n_edges_orig}\n"), file_name, append = FALSE)

  for(e in edges) {
    write(paste0(e), file_name, append = TRUE)
  }
}

for (idx in seq(n_graphs)) {
  generate_graph(file_path = file_path, idx = idx, max_vert = max_vert)
}
