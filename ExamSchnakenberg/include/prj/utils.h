#pragma once

#include <set>
#include <vector>

std::vector<std::set<int>> get_all_subsets(const std::set<int>& inputSet,
                                           int subset_size);

std::vector<std::set<int>> get_all_subsets_with_size(
    const std::set<int>& inputSet, int subset_size_lower_bound,
    int subset_size_upper_bound);