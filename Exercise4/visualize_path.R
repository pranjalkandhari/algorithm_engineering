library(tidyverse)
library(ggplot2)
library(magrittr)

in_path <- "~/dev/priv/master/algorithm_engineering/code/exercise4/input/"
in_files <- fs::dir_ls(in_path)
out_files <- fs::dir_info("~/dev/priv/master/algorithm_engineering/code/exercise4/out/") %>% 
  arrange(modification_time) %>% 
  pull(path)

read_tsp_input <- function(file) {
  readLines(file) %>% 
    purrr::keep(\(l) {
      grepl(r"(\d\s[0-9\.]+\s[0-9\.]+)", l )
    }) %>% 
    purrr::map(\(l) {
      stringr::str_split(l, "\\s")[[1]]
    }) %>% 
    purrr::transpose() %>% 
    purrr::map(as.character) %>% 
    as.data.frame() %>% 
    setNames(c("idx", "x", "y")) %>% 
    dplyr::mutate(
      idx = as.integer(idx),
      x = as.double(x),
      y = as.double(y)#
    ) %>% 
    tibble::as_tibble()
}

plot_tsp_input <- function(tsp, edges, title) {
  
  df <- edges %>% 
    inner_join(
      tsp %>% setNames(c("idx", "xfrom", "yfrom")), by = c("from" = "idx")
    )%>% 
    inner_join(
      tsp %>% setNames(c("idx", "xto", "yto")), by = c("to" = "idx")
    )
  
  ggplot() +
    geom_point(data = tsp, aes(x = x, y = y), size = 0.5) + 
    geom_segment(
      data = df,
      aes(x = xfrom, y = yfrom, xend = xto, yend = yto),
      arrow = grid::arrow(type = "closed", angle = 30, length = unit(0.03, "inches"))
    ) +
    geom_text(data = tsp, aes(x = x, y = y, label = idx), size = 1, nudge_x = 0, nudge_y = 0,  color = "red") +
    ggtitle(title)
}

read_route_order <- function(file) {
  read_csv(file, col_types = "iid")
}

validate <- function(in_file, out_file) {
  inlines <- readLines(in_file) 
  dim_line <- inlines[grepl("DIMENSION", inlines)]
  dim <- as.integer(stringr::str_remove(dim_line, "DIMENSION : "))
  stopifnot(!is.na(dim) || length(dim) == 1)
  
  from <- integer(dim)
  to <- integer(dim)
  
  edges <- read_route_order(out_file)
  
  if(anyDuplicated(edges$from)) {
    rlang::abort("from duplicated")
  }
  
  if(anyDuplicated(edges$to)) {
    rlang::abort("to duplicated")
  }
  
  if(length(edges$from) + length(edges$to) != 2 * dim) {
    rlang::abort("Edge missing")
  }
  
  if(any(c(edges$from, edges$to) <= 0 | c(edges$from, edges$to) > dim)) {
    rlang::abort("invalid edge idx")
  }
  
  if(any(is.na(c(edges$from, edges$to)))) {
    rlang::abort("NA edge")
  }
  
  if(nrow(filter(edges, from == to)) > 0) {
    rlang::abort("from == to")
  }
}


instance <- "bm33708"

in_file <- in_files[grepl(instance, in_files)]
out_files <- out_files[grepl(instance, out_files)]
start_file <- out_files[grepl(paste0(instance, ".tsp_0"), out_files) ]
end_file <- out_files[grepl(paste0(instance, ".tsp$"), out_files)]
tsp <- read_tsp_input(in_file)

validate(in_file, start_file)
validate(in_file, end_file)


for (of in c(start_file, end_file)) {
  fname <- tail(stringr::str_split(of, "/")[[1]], 1)
  
  edges <- read_route_order(of)
  gr_plot <- plot_tsp_input(tsp, edges, fname)
  ggsave(paste0(fname, ".png"), gr_plot, dpi = 1500)
  # ggsave(paste0(fname, ".png"), gr_plot)
}
