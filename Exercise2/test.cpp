//
// Created by Nils Schnakenberg on 23.11.23.
//

#include <iostream>
#include <vector>

void print_vec(std::vector<int> data) {
    for(auto i: data) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int binary_search_index(std::vector<int> &v, int f) {
    auto it = std::lower_bound(v.begin(), v.end(), 6);
    return std::distance(v.begin(), it);
}

int main() {

//    std::vector<int> v;
//
//    for (int i = 0; i < 10; i++) {
//        if(i != 6) {
//
//            v.push_back(i);
//        }
//    }

    std::vector<int> v = {2, 3};

    print_vec(v);

    auto x = binary_search_index(v, 0);

    std::cout << x << std::endl;

    std::cout << "Hello World" << std::endl;
}