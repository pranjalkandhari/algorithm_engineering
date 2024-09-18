#include<iostream>
#include<algorithm>
#include<vector>
#include<ctime>
#include<omp.h>
#include<map>
#include<random>
#include<chrono>
#include<cstring>
#define ll long long
using namespace std;


void swap(ll &a, ll &b) {
    ll temp = a;
    a = b;
    b = temp;
}

// It partition the array by creating a random pivot sequencially
ll partition_sequencial(vector<ll> &array, ll start, ll end) {

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(start, end); // define the range

    ll pivot_index = distr(gen);
    //pivot_index = start;
    ll below_ctr = 0; // Counts number of elements smaller than the pivot element.
    ll above_ctr = 0; // Counts number of elements larger and equal to the pivot element.
    
    for (ll i=start; i<=end; i++) {
        if (array[i] < array[pivot_index]) {
            below_ctr++;
        } else {
            above_ctr++;
        }
    }

    // Putting pivot element to it's correct place an updating pivot_index:
    swap(array[pivot_index], array[start+below_ctr]);
    pivot_index = start+below_ctr;

    // Putting elements smaller than pivot before it and larger and equal after it.
    ll i=start, j=pivot_index+1;
    while(i<pivot_index && j<=end) {
        if (array[i] >= array[pivot_index] && array[j] < array[pivot_index]) {
            swap(array[i], array[j]);
            i++;
            j++;
        } else if (array[i] >= array[pivot_index]) {
            j++;
        } else if(array[j] < array[pivot_index]) {
            i++;
        } else {
            i++;
            j++;
        }
    }
    return pivot_index;
}

// It partition the array by creating a random pivot parallely (p: number of processors):
ll partition_parallel(vector<ll> &array, ll start, ll end, ll p) {

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(start, end); // define the range

    ll pivot_index = distr(gen);
    ll pivot_element = array[pivot_index];
    ll partition_size = (end-start+1)/p;
    if (partition_size == 0) {
        partition_size++;
    }
    map< pair<ll,ll> , pair<ll, ll> > range_count_map; // Map from range to count.
    
    #pragma omp parallel for num_threads(p)
    for (ll p_start = start; p_start<=end; p_start += partition_size) {
        ll processor_number = omp_get_thread_num();
        ll p_end = p_start + partition_size - 1;
        if (p_end > end) {
            p_end = end;
        }
        // Now task of one processor is to store the number of elements below and above the pivot in the map for range p_start to p_end.
        ll below_ctr = 0;
        ll above_ctr = 0;
        for (ll i=p_start; i<=p_end; i++) {
            if (array[i] < array[pivot_index]) {
                below_ctr++;
            } else {
                above_ctr++;
            }
        }

        // Making this line sequential as map can only be used once, but it only runs p times so does not affect the time complexity.
        #pragma omp critical
        {
            range_count_map[make_pair(p_start, p_end)] = make_pair(below_ctr, above_ctr);
        }
    }
    
    ll *partitioned_elements = new ll[end-start +1];

    map< pair<ll,ll>, pair<ll,ll> > range_pre_sum_map; // Maps all ranges to (presum below elements, presum above elements).
    ll sum_below = 0;
    ll sum_above = 0;
    ll to_swap = -1;

    // Sequential loop of O(p) iterations.
    for (ll p_start = start; p_start<=end; p_start += partition_size) {
        ll p_end = p_start + partition_size - 1;
        if (p_end > end) {
            p_end = end;
        }
        range_pre_sum_map[make_pair(p_start, p_end)] = make_pair(sum_below, sum_above);
        sum_below += range_count_map[make_pair(p_start, p_end)].first;
        sum_above += range_count_map[make_pair(p_start, p_end)].second;
    }

    #pragma omp parallel for num_threads(p)
    for (ll p_start = start; p_start<=end; p_start += partition_size) {
        ll p_end = p_start + partition_size - 1;
        if (p_end > end) {
            p_end = end;
        }
        ll processor_number = omp_get_thread_num();

        pair<ll, ll> pre_sum_pair;

        // Making this line sequential as map can only be used once, but it only runs p times so does not affect the time complexity.
        #pragma omp critical
        {
            pre_sum_pair = range_pre_sum_map[make_pair(p_start, p_end)];
        }
        
        ll below_written = 0;
        ll above_written = 0;
        for (ll i=p_start; i<=p_end; i++) {
            if (array[i] < pivot_element) {
                ll index_in_part_array = pre_sum_pair.first + below_written;
                partitioned_elements[index_in_part_array] = array[i];
                below_written++;
            } else {
                ll index_in_part_array = pre_sum_pair.second + above_written + sum_below;
                if (array[i] == pivot_element) {
                    to_swap = index_in_part_array;
                }
                partitioned_elements[index_in_part_array] = array[i];
                above_written++;
            }
        }
    }

    swap(partitioned_elements[sum_below], partitioned_elements[to_swap]);

    for (ll p_start = start; p_start<=end; p_start += partition_size) {
        ll processor_number = omp_get_thread_num();
        ll p_end = p_start + partition_size - 1;
        if (p_end > end) {
            p_end = end;
        }
        for (ll i=p_start; i<=p_end; i++) {
            array[i] = partitioned_elements[i-start];
        }
    }
    
    return start + sum_below;
}

// Function to sort. 
// is_parallel: true when parallel. p: Number of processors (relavent only when is_parallel is true).
void quick_sort(vector<ll> &array, ll start, ll end, bool is_parallel, ll p) {   
    // Base case:
    if (start >= end) {
        return;
    }

    // Partition array and get pivot index.
    ll pivot_index = -1;
    if (!is_parallel) {
        pivot_index = partition_sequencial(array, start, end);
    } else {
        pivot_index = partition_parallel(array, start, end, p);
    }

    // Sorting both the sides of the pivot:
    quick_sort(array, start, pivot_index-1, is_parallel, p);
    quick_sort(array, pivot_index+1, end, is_parallel, p);
}

// Function to compare 2 vectors: true if they are same.
bool compare_2_arrays(vector<ll> array1, vector<ll> array2) {
    
    if (array1.size() != array2.size()) {
        return false;
    }
    for (int i = 0; i<array1.size(); i++) {
        if (array1[i] != array2[i]) {
            return false;
        }
    }
    return true;
}

// Test function for classical merge sort. Returns time to run.
// p: number of processors.
pair<bool, ll> test_sort(ll p, ll n, ll is_parallel) {
    
    ll max_element = 100000;

    cout<<"Input array size: "<<n<<endl;

    vector<ll> test_array_1;
    vector<ll> test_array_2;

    for (int i=0; i<n; i++) {

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(1, max_element); // define the range

        ll new_element = distr(gen);
        test_array_1.push_back(new_element);
        test_array_2.push_back(new_element);
    }

    // Sorting test_array_1 using quick sort:
    auto start_time = std::chrono::high_resolution_clock::now();
    quick_sort(test_array_1, 0, n-1, is_parallel, p);
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calling inbuilt calling function to compare:
    sort(test_array_2.begin(), test_array_2.end());

    bool comparision_result = compare_2_arrays(test_array_1, test_array_2);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    ll total_time = ((ll)duration.count())/1000 ;
    cout<<"Total time (ms) "<<total_time<<endl;
    
    return make_pair(comparision_result, total_time);
}

int main(int argc, char* argv[]) {
    // Check if there are enough command-line arguments
    if (argc < 4) {
        string message_str = " <Number of processors> <Number of elements> <True/False for parallel sorting>";
        std::cerr<<"Usage: "<<argv[0]<<message_str<<endl;
        return 1; // Exit with an error code
    }

    // Convert command-line arguments to integers
    ll p = std::atoi(argv[1]);
    ll n = std::atoi(argv[2]);

    bool is_parallel = (strcmpi(argv[3], "true") == 0);

    cout<<"Number of cores: "<<p<<endl;
    cout<<"Number of elements: "<<n<<endl;
    cout<<"Parallelization: ";
    if (is_parallel) {
        cout<<"True"<<endl;
    } else {
        cout<<"False"<<endl;
    }

    pair<bool, ll> test_result = test_sort(p, n, is_parallel);

    if (test_result.first) {
        cout<<"Sorting successfull"<<endl;
    } else {
        cout<<"Sorting falied"<<endl;
    }
    cout<<"Time taken (in ms): "<<test_result.second<<endl;
    return 0;
}
