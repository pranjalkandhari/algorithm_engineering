#include <vector>
#include <random>
#include <algorithm>

#include "limits.h"
#include "random_numbers.h"

std::vector<int> gen_random_integers(long long n_integers) {
    // initialize random generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);

    std::vector<int> random_integers;
    random_integers.reserve(n_integers);

    for (int i = 0; i < n_integers; ++i) {
        int random_int = dist(mt);
        random_integers.push_back(random_int);
    }
    return random_integers;
}

std::vector<int> gen_non_duplicate_random_integers(long long n_integers) {
    std::vector<int> v;
    v.reserve(n_integers);

    for (int i = 0; i < n_integers; ++i) {
        v.push_back(i);
    }

    auto rng = std::default_random_engine {};

    std::shuffle(std::begin(v), std::end(v), rng);
    return v;
}
