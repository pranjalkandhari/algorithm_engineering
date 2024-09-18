#include <iostream>
#include <numeric>

#include "utils.h"

void print_vec(std::vector<int> data) {
    for(auto i: data) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

long long average(std::vector<long long> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<long long>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}