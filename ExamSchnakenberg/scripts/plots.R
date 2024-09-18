library(ggplot2)
library(readr)
library(magrittr)
library(dplyr)
library(scales)


################################################################################
# Data Preparation
################################################################################

optimal <- read_csv(
  "~/dev/priv/master/algorithm_engineering/code/exam_steiner/input/track1.csv",
  col_types = cols(paceName = "c", opt = "n")
) %>% 
  dplyr::rename(file = paceName, optimal_steiner_weight = opt)

approx <- read_csv(
  "~/dev/priv/master/algorithm_engineering/code/exam_steiner/results/approximation_report.csv",
  col_types = cols_only(
    file = "c",
    graph_nodes = "i",
    graph_edges = "i",
    graph_weight = "n",
    terminals = "i",
    # parse_time = "i",
    steiner_weight = "n",
    steiner_time = "i"
  )) %>% 
  dplyr::rename(
    approximation_steiner_weight = steiner_weight,
    approximation_steiner_time = steiner_time
  )

approx_parallel <- read_csv(
  "~/dev/priv/master/algorithm_engineering/code/exam_steiner/results/approximation_parallel_report.csv",
  col_types = cols_only(
    file = "c",
    steiner_weight = "n",
    steiner_time = "i"
  )) %>% 
  dplyr::rename(
    approximation_parallel_steiner_weight = steiner_weight,
    approximation_parallel_steiner_time = steiner_time
  )


heuristic <- read_csv(
  "~/dev/priv/master/algorithm_engineering/code/exam_steiner/results/heuristic_report.csv",
  col_types = cols_only(
    file = "c",
    steiner_weight = "n",
    steiner_time = "i"
  )) %>% 
  dplyr::rename(
    heuristic_steiner_weight = steiner_weight,
    heuristic_steiner_time = steiner_time
  )

exact <- read_csv(
  "~/dev/priv/master/algorithm_engineering/code/exam_steiner/results/exact_report.csv",
  col_types = cols_only(
    file = "c",
    steiner_weight = "n",
    steiner_time = "i"
  )) %>% 
  dplyr::rename(
    exact_steiner_weight = steiner_weight,
    exact_steiner_time = steiner_time
  )


comb_result <- optimal %>% 
  dplyr::left_join(
    approx,
    by = "file"
  ) %>% 
  dplyr::left_join(
    approx_parallel,
    by = "file"
  ) %>% 
  dplyr::left_join(
    heuristic,
    by = "file"
  ) %>% 
  dplyr::left_join(
    exact,
    by = "file"
  ) %>% 
  dplyr::mutate(
    approximation_optimality = approximation_steiner_weight / optimal_steiner_weight,
    heuristic_optimality = heuristic_steiner_weight / optimal_steiner_weight,
    approximation_heuristic_optimality = approximation_optimality / heuristic_optimality,
    parallel_speedup = approximation_parallel_steiner_time / approximation_steiner_time,
    approximation_heuristic_comp = approximation_steiner_time / heuristic_steiner_time,
    parallel_approximation_heuristic_comp = approximation_parallel_steiner_time / heuristic_steiner_time,
    
    term_vert_squared = terminals * (graph_nodes ^ 2)
  )

# TODO validate all dplyr::filter(approximation_steiner_weight == approximation_parallel_steiner_weight)
# TODO validate dplyr::filter(exact_steiner_weight != -1) %>% dplyr::filter(exact_steiner_weight == optimal_steiner_weight)



################################################################################
# Plot Solution Quality
################################################################################


comb_result_optimality <- comb_result %>% 
  dplyr::select(file, Approximation = approximation_optimality, Heuristic = heuristic_optimality) %>% 
  tidyr::pivot_longer(
    cols = c(Approximation, Heuristic),
    names_to = "Algorithm",
    values_to = "optimality"
  )

comb_result_optimality_order <- comb_result %>% 
  dplyr::arrange(heuristic_optimality) %>% 
  dplyr::pull(file)

optimality_plot <- ggplot(comb_result_optimality, aes(fill = Algorithm, x = file, y = optimality)) +
  geom_bar(stat = "identity", position = "dodge", width = 1) +
  theme(
    axis.text.x = element_text(angle = 90, vjust = 0.5, hjust=1),
    legend.title = element_text(size=30),
    legend.text = element_text(size=23)
    ) +
  scale_y_continuous(limits = c(1,2), oob = rescale_none) +
  scale_x_discrete(limits = comb_result_optimality_order) +
  scale_fill_brewer(palette="Set1") +
  xlab("Challenge Index") +
  ylab("Optimality")

ggsave("optimality_plot.png", optimality_plot, width = 30, height = 10, dpi = 400)

approximation_optimality_plot <- ggplot(comb_result, aes(x = approximation_optimality)) +
  geom_histogram(bins = 60) +
  geom_density(fill = 3, alpha = 0.4) +
  xlab("Approximation Optimality")

ggsave("approximation_optimality_plot.png", approximation_optimality_plot, width = 10, height = 7, dpi = 400)


heuristic_optimality_plot <- ggplot(comb_result, aes(x = heuristic_optimality)) +
  geom_histogram(bins = 60) +
  geom_density(fill = 3, alpha = 0.4) +
  xlab("Heuristic Optimality")

ggsave("heuristic_optimality_plot.png", heuristic_optimality_plot, width = 10, height = 7, dpi = 400)


optimality_comparistion_order <- comb_result %>% 
  dplyr::arrange(approximation_heuristic_optimality) %>% 
  dplyr::pull(file)

optimality_comparison_plot <- ggplot(comb_result, aes(x = file, y = approximation_heuristic_optimality)) +
  geom_bar(stat = "identity", position = "dodge", width = 1) +
  theme(
    axis.text.x = element_blank(), #element_text(angle = 90, vjust = 0.5, hjust=1),
    legend.title = element_text(size=30),
    legend.text = element_text(size=23)
  ) +
  scale_y_continuous(limits = c(0.8, 1.2), oob = rescale_none, breaks = seq(0.8, 1.2, 0.05)) +
  scale_x_discrete(limits = optimality_comparistion_order) +
  scale_fill_brewer(palette="Set1") +
  xlab("Challenge Index") +
  ylab("Solution Quality: Heuristic / Approximation")

ggsave("optimality_comparison_plot.png", optimality_comparison_plot, width = 10, height = 7, dpi = 400)

################################################################################
# Plot running times
################################################################################



comb_result_running_time <- comb_result %>% 
  dplyr::select(
    file,
    Exact = exact_steiner_time,
    Approximation = approximation_steiner_time, 
    `Approximation Parallel` = approximation_parallel_steiner_time,
    Heuristic = heuristic_steiner_time) %>% 
  tidyr::pivot_longer(
    cols = c(Exact, Approximation, `Approximation Parallel`, Heuristic),
    names_to = "Algorithm",
    values_to = "running_time"
  ) %>% 
  dplyr::mutate(running_time = log10(running_time))

comb_result_running_time_order <- comb_result %>% 
  dplyr::arrange(file) %>% 
  dplyr::pull(file)

running_time_plot <- ggplot(comb_result_running_time, aes(fill = Algorithm, x = file, y = running_time)) +
  geom_bar(stat = "identity", position = "dodge", width = 1) +
  theme(
    axis.text.x = element_text(angle = 90, vjust = 0.5, hjust=1),
    legend.title = element_text(size=30),
    legend.text = element_text(size=23)
  ) +
  scale_y_continuous(labels = \(x) {(10 ^ x) / 1e6}) +
  scale_x_discrete(limits = comb_result_running_time_order) +
  scale_fill_brewer(palette="Set1") +
  xlab("Challenge Index") +
  ylab("Running Time in s")

ggsave("running_time_plot.png", running_time_plot, width = 30, height = 10, dpi = 400)


approximation_heuristic_time_order <- comb_result %>% 
  dplyr::arrange(graph_nodes * terminals ^ 2) %>% 
  dplyr::pull(file)

approximation_heuristic_time_plot <- ggplot(comb_result, aes(x = file, y = approximation_heuristic_comp)) +
  geom_bar(stat = "identity", width = 1) +
  theme(
    axis.text.x = element_blank(),#element_text(angle = 90, vjust = 0.5, hjust=1),
    legend.title = element_text(size=30),
    legend.text = element_text(size=23)
  ) +
  scale_y_continuous(breaks = seq(0, 10, 1), limits = c(0, 10), oob = rescale_none) +
  scale_x_discrete(limits = approximation_heuristic_time_order) +
  scale_fill_brewer(palette="Set1") +
  xlab("Challenge Index") +
  ylab("Running Time: Approximation / Heuristic")

ggsave("approximation_heuristic_time_plot.png", approximation_heuristic_time_plot, width = 10, height = 7, dpi = 400)


parallel_speedup_time_order <- comb_result %>% 
  dplyr::arrange(parallel_speedup) %>% 
  dplyr::pull(file)

parallel_speedup_time_plot <- ggplot(comb_result, aes(x = file, y = parallel_speedup)) +
  geom_bar(stat = "identity", width = 1) +
  theme(
    axis.text.x = element_blank(),#element_text(angle = 90, vjust = 0.5, hjust=1),
    legend.title = element_text(size=30),
    legend.text = element_text(size=23)
  ) +
  scale_y_continuous(breaks = seq(0, 3, 0.5), limits = c(0, 3), oob = rescale_none) +
  scale_x_discrete(limits = parallel_speedup_time_order) +
  scale_fill_brewer(palette="Set1") +
  xlab("Challenge Index") +
  ylab("Running Time: Parallel Approximation / Approximation")

ggsave("parallel_speedup_time_plot.png", parallel_speedup_time_plot, width = 10, height = 7, dpi = 400)


exact_steiner_theoretical_plot <- ggplot(comb_result %>% 
                                                   dplyr::mutate(log_approximation_steiner_time = log10(approximation_steiner_time)), 
                                                 aes(x = term_vert_squared, y = log_approximation_steiner_time)) +
  geom_point() +
  scale_y_continuous(labels = \(x) {(10 ^ x) / 1e6}) +
  scale_x_log10(labels = scales::number_format()) +
  xlab("|S| * (|V|) ^ 2") +
  ylab("Running Time in s") + 
  geom_smooth()

ggsave("exact_steiner_theoretical_plot.png", exact_steiner_theoretical_plot, width = 10, height = 7, dpi = 400)



approximation_steiner_theoretical_plot <- ggplot(comb_result %>% 
         dplyr::mutate(log_approximation_steiner_time = log10(approximation_steiner_time)), 
       aes(x = term_vert_squared, y = log_approximation_steiner_time)) +
  geom_point() +
  scale_y_continuous(labels = \(x) {(10 ^ x) / 1e6}) +
  scale_x_log10(labels = scales::number_format()) +
  xlab("|S| * (|V|) ^ 2") +
  ylab("Running Time in s") + 
  geom_smooth()

ggsave("approximation_steiner_theoretical_plot.png", approximation_steiner_theoretical_plot, width = 10, height = 7, dpi = 400)


approximation_parallel_steiner_theoretical_plot <- ggplot(comb_result %>% 
         dplyr::mutate(log_approximation_parallel_steiner_time = log10(approximation_parallel_steiner_time)), 
       aes(x = term_vert_squared, y = log_approximation_parallel_steiner_time)) +
  geom_point() +
  scale_y_continuous(labels = \(x) {(10 ^ x) / 1e6}) +
  scale_x_log10(labels = scales::number_format()) +
  xlab("|T| * (|V|) ^ 2") +
  ylab("Running Time in s") + 
  geom_smooth()

ggsave("approximation_parallel_steiner_theoretical_plot.png", approximation_parallel_steiner_theoretical_plot, width = 10, height = 7, dpi = 400)

heuristic_steiner_theoretical_plot <- ggplot(comb_result %>% 
                                                            dplyr::mutate(log_heuristic_steiner_time = log10(heuristic_steiner_time)), 
                                                          aes(x = term_vert_squared, y = log_heuristic_steiner_time)) +
  geom_point() +
  scale_y_continuous(labels = \(x) {(10 ^ x) / 1e6}) +
  scale_x_log10(labels = scales::number_format()) +
  xlab("|T| * (|V|) ^ 2") +
  ylab("Running Time in s") + 
  geom_smooth()

ggsave("heuristic_steiner_theoretical_plot.png", heuristic_steiner_theoretical_plot, width = 10, height = 7, dpi = 400)


parallelization_break_even <- ggplot(comb_result, 
       aes(x = graph_nodes, y = parallel_speedup)) +
  geom_point() +
  ylim(0,3) +
  xlim(0, 5000) +
  xlab("|Nodes|") +
  ylab("Parallel Time / Serial Time") +
  geom_smooth()

ggsave("parallelization_break_even.png", parallelization_break_even, width = 10, height = 7, dpi = 400)

################################################################################
# Tables
################################################################################

# Dataset Properties

vtable::sumtable(
  comb_result, 
  vars = c("graph_nodes", "graph_edges", "terminals"),
  labels = c("Nodes", "Edges", "Terminals"),
  numformat = vtable::formatfunc(big.mark = ","),
  summ = c('length(x)','mean(x)','sd(x)','min(x)','pctile(x)[25]', 'pctile(x)[50]', 'pctile(x)[75]','max(x)'),
  summ.names = c('N','Mean','Std. Dev.','Min','Pctl. 25', 'Median', 'Pctl. 75','Max'),
  out = "latex"
)

# Solution Quality

vtable::sumtable(
  comb_result, 
  vars = c("optimal_steiner_weight", "approximation_optimality", "heuristic_optimality"), 
  labels = c("Minimal Steiner tree size", "Approximation", "Heuristic"),
  numformat = vtable::formatfunc(big.mark = ","),
  summ = c('length(x)','mean(x)','sd(x)','min(x)','pctile(x)[25]', 'pctile(x)[50]', 'pctile(x)[75]','max(x)'),
  summ.names = c('N','Mean','Std. Dev.','Min','Pctl. 25', 'Median', 'Pctl. 75','Max'),
  out = "latex"
)


# Running times (with exact)

vtable::sumtable(
  comb_result %>% 
    dplyr::filter(exact_steiner_weight != -1) %>% 
    dplyr::mutate(
      dplyr::across(
        c("exact_steiner_time", "approximation_steiner_time", "approximation_parallel_steiner_time", "heuristic_steiner_time"),
        \(x) x / 1e6
      )
    ), 
  vars = c("exact_steiner_time", "approximation_steiner_time", "approximation_parallel_steiner_time", "heuristic_steiner_time"), 
  labels = c("Exact", "Approximation", "Parallel Approximation", "Heuristic"),
  numformat = vtable::formatfunc(big.mark = ","),
  summ = c('length(x)','mean(x)','sd(x)','min(x)','pctile(x)[25]', 'pctile(x)[50]', 'pctile(x)[75]','max(x)'),
  summ.names = c('N','Mean','Std. Dev.','Min','Pctl. 25', 'Median', 'Pctl. 75','Max'),
  out = "latex"
)

# Running times (without exact)

vtable::sumtable(
  comb_result %>% 
    dplyr::mutate(
      dplyr::across(
        c("approximation_steiner_time", "approximation_parallel_steiner_time", "heuristic_steiner_time"),
        \(x) x / 1e6
      )
    ), 
  vars = c("approximation_steiner_time", "approximation_parallel_steiner_time", "heuristic_steiner_time"), 
  labels = c("Approximation", "Parallel Approximation", "Heuristic"),
  numformat = vtable::formatfunc(big.mark = ","),
  summ = c('length(x)','mean(x)','sd(x)','min(x)','pctile(x)[25]', 'pctile(x)[50]', 'pctile(x)[75]','max(x)'),
  summ.names = c('N','Mean','Std. Dev.','Min','Pctl. 25', 'Median', 'Pctl. 75','Max'),
  out = "latex"
)


