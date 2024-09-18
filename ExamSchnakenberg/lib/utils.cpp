#include "../include/prj/utils.h"

// Function to generate all possible subsets of size  from a given set
void generate_subsets(const std::set<int>& inputSet, int subset_size, int index,
                      std::vector<int>& current,
                      std::vector<std::set<int>>& result) {
  if (current.size() == subset_size) {
    //    result.push_back(current);
    result.push_back(std::set<int>(current.begin(), current.end()));
    return;
  }

  // If no more elements are there to put in current[]
  if (index == inputSet.size()) return;

  // Include current element in current[]
  current.push_back(*std::next(inputSet.begin(), index));
  generate_subsets(inputSet, subset_size, index + 1, current, result);

  // Backtrack: Remove current element from current[]
  current.pop_back();

  // Explore remaining elements
  generate_subsets(inputSet, subset_size, index + 1, current, result);
}

// Function to call generateSubsets and return the result
std::vector<std::set<int>> get_all_subsets(const std::set<int>& inputSet,
                                           int subset_size) {
  std::vector<std::set<int>> result;
  std::vector<int> current;
  generate_subsets(inputSet, subset_size, 0, current, result);
  return result;
}

std::vector<std::set<int>> get_all_subsets_with_size(
    const std::set<int>& inputSet, int subset_size_lower_bound,
    int subset_size_upper_bound) {
  std::vector<std::set<int>> result;
  for (int i = subset_size_lower_bound; i <= subset_size_upper_bound; i++) {
    std::vector<std::set<int>> subsets = get_all_subsets(inputSet, i);
    result.insert(result.end(), subsets.begin(), subsets.end());
  }
  return result;
}
