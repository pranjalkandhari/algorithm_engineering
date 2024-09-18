#include<iostream>
#include<cmath>
#include<vector>
#include<set>
#include<chrono>
#include<ctime>
#include<string>
#include<fstream>
using namespace std;

long long time_limit = 5*60*1000; // time limit to run the 2 opt algorithm in mili seconds.
string BASE_PATH_INPUT = "input_data/";
string BASE_PATH_OUTPUT = "output_data/";
string OUTPUT_EXTENTION = ".txt";
int total_2opt_iterations = 0;
long two_opt_edge_swaps = 0;
vector<string> FILE_NAMES = {
        "01_Argentina.txt",
        "02_Burma.txt",
        "03_China.txt",
        "04_Djibouti.txt",
        "05_Egypt.txt",
        "06_Finland.txt",
        "07_Greece.txt",
        "08_Honduras.txt",
        "09_Ireland.txt",
        "10_Japan.txt",
        "11_Kazakhstan.txt",
        "12_Luxembourg.txt",
        "13_Morocco.txt",
        "14_Nicaragua.txt",
        "15_Oman.txt",
        "16_Panama.txt",
        "17_Qatar.txt",
        "18_Rwanda.txt",
        "19_Sweden.txt",
        "20_Tanzania.txt",
        "21_Uruguay.txt",
        "22_Vietnam.txt",
        "23_Western_Sahara.txt",
        "24_Yemen.txt",
        "25_Zimbabwe.txt",
        "26_Canada.txt",
        "27_Italy.txt"
    };

// To find the euclidean distance between 2 points: (x1,y1) and (x2,y2).
float find_distance(float x1, float y1, float x2, float y2);

// Function to make calls to neareast neighbor and 2 opt algorithms for the given file name
void tsp_runner(string file_name);

// To find the approximate solution for the tsp: Takes points, starting point, empty set, empty path which is filled.
void nearest_neighbor_tsp(vector<pair<float,float> > &points, int start, set<int> &covered_points, vector<int> &path);

// To set the total iterations required in the 2 opt algorithm.
void set_2opt_iterations(vector<pair<float, float> > &points);

// To reverse vector from start to end both inclusive
void reverse_vector_segment(vector<int> &path, int start, int end);

// To find path length for the round tour
double find_path_length(vector<pair<float, float> > points, vector<int> path);

// To find the approximate solution for the tsp: Takes points and returns a path.
vector<int> two_opt_tsp(vector<pair<float, float> > &points, vector<int> initial_path, vector<int> &save_path_1, vector<int> &save_path_2);

// To test if the path is a feasible path. n: number of points given in input.
bool test_tsp_path(vector<int> &path, int n);

void print_path(vector<int> path);

long long get_current_time();

void write_path_to_file(vector<int> &path, vector<pair<float, float> > &points, string file_name);

string get_file_name_without_extension(const string &file_path);


int main() {
    vector<string> file_names = FILE_NAMES;
    for (int i=0; i<file_names.size(); i++) {
        tsp_runner(file_names[i]);
    }
}


void tsp_runner(string file_name) {
    string file_path = BASE_PATH_INPUT + file_name;
    freopen(file_path.c_str(), "r", stdin);
    cout<<"File path: "<<file_path<<endl;
    int n;
    cin>>n;
    cout<<"n: "<<n<<endl;
    vector<pair<float, float> > points;
    set<int> covered_set;
    for (int i=0; i<n; i++) {
        float x,y;
        cin>>x>>y;
        points.push_back(make_pair(x,y));
    }
    cout<<"points size: "<<points.size()<<endl;

    // Nearest neighbor algorithm call:
    vector<int> path_nearest_neighbor;
    long long nn_time_1 = get_current_time();
    nearest_neighbor_tsp(points, 0, covered_set, path_nearest_neighbor);
    long long nn_time_2 = get_current_time();
    write_path_to_file(path_nearest_neighbor, points, get_file_name_without_extension(file_name) + "_nn_a" + OUTPUT_EXTENTION);
    cout<<"Path length by nearest neighbor algorithm: "<<find_path_length(points, path_nearest_neighbor)<<endl;
    cout<<"Time taken: "<<nn_time_2-nn_time_1<<endl<<endl;
    if (test_tsp_path(path_nearest_neighbor, points.size())) {
        cout<<"NEAREST NEIGHBOR TEST PASSED"<<endl;
    } else {
        cout<<"NEAREST NEIGHBOR TEST FAILED"<<endl;
    }

    // 2 ops algortihm call:
    cout<<"2 OPT Algorithm with default initial path:"<<endl;
    vector<int> empty_path;
    vector<int> initial_path;
    vector<int> mid_path;

    set_2opt_iterations(points);
    long long two_opt_time_1 = get_current_time();
    vector<int> path_2opt = two_opt_tsp(points, empty_path, initial_path, mid_path);
    long long two_opt_time_2 = get_current_time();
    write_path_to_file(initial_path, points, get_file_name_without_extension(file_name) + "_2opt_b" + OUTPUT_EXTENTION);
    write_path_to_file(mid_path, points, get_file_name_without_extension(file_name) + "_2opt_c" + OUTPUT_EXTENTION);
    write_path_to_file(path_2opt, points, get_file_name_without_extension(file_name) + "_2opt_d" + OUTPUT_EXTENTION);
    cout<<"Path length by 2opt algorithm: "<<find_path_length(points, path_2opt)<<endl;
    cout<<"Time taken wirh initial input as default: "<<two_opt_time_2-two_opt_time_1<<endl;
    if (test_tsp_path(path_2opt, points.size())) {
        cout<<"2 OPT TEST PASSED"<<endl;
    } else {
        cout<<"2 OPT TEST FAILED"<<endl;
    }
    cout<<"Number of edge swaps = "<<two_opt_edge_swaps<<endl<<endl;

    cout<<"2 OPT Algorithm with default nearest neighbor path:"<<endl;
    vector<int> initial_path_nn;
    vector<int> mid_path__nn;
    two_opt_time_1 = get_current_time();
    vector<int> path_2opt_nn = two_opt_tsp(points, path_nearest_neighbor, initial_path_nn, mid_path__nn);
    two_opt_time_2 = get_current_time();
    write_path_to_file(path_2opt_nn, points, get_file_name_without_extension(file_name) + "_2opt_e" + OUTPUT_EXTENTION);
    cout<<"Path length by 2opt algorithm: "<<find_path_length(points, path_2opt_nn)<<endl;
    cout<<"Time taken for 2 opt with initial input from nearest neighbor: "<<two_opt_time_2-two_opt_time_1<<endl;
    if (test_tsp_path(path_2opt_nn, points.size())) {
        cout<<"2 OPT with nearest neighbour input TEST PASSED"<<endl;
    } else {
        cout<<"2 OPT with nearest neighbour input TEST FAILED"<<endl;
    }
    cout<<"Number of edge swaps = "<<two_opt_edge_swaps<<endl<<endl;
    cout<<"-----------------------------"<<endl;
}


void nearest_neighbor_tsp(vector<pair<float,float> > &points, int current, set<int> &covered_points, vector<int> &path) {

    path.push_back(current);
    covered_points.insert(current);
    while(covered_points.size() < points.size()) {
        float min_distance = 10000000000;
        int min_index = -1;
        for (int i=0; i<points.size(); i++) {
            if (i == current || covered_points.find(i) != covered_points.end()) {
                continue;
            }
            float distance = find_distance(points[current].first, points[current].second, points[i].first, points[i].second);
            if (min_index == -1) {
                min_index = i;
                min_distance = distance;
            }
            
            if (distance < min_distance) {
                min_index = i;
                min_distance = distance;
            }
        }
        path.push_back(min_index);
        covered_points.insert(min_index);
        
        current = min_index;
    }
    path.push_back(path[0]);
}


vector<int> two_opt_tsp(vector<pair<float, float> > &points, vector<int> initial_path, vector<int> &save_path_1, vector<int> &save_path_2) {
    two_opt_edge_swaps = 0;
    vector<int> path;
    for (int i=0; i<points.size(); i++) {
        path.push_back(i);
    }
    
    if (path.size() < 4) {
        path.push_back(path[0]);
        return path;
    }
    path.push_back(path[0]);

    if (initial_path.size() > 0) {
        path = initial_path;
    }

    for (int i=0; i<path.size(); i++) {
        save_path_1.push_back(path[i]);
    }
    
    double path_length = find_path_length(points, path);
    long long initial_time = get_current_time();
    long long current_time = get_current_time();
    bool saved_mid_path = false;
    int iteration_ctr = 0;
    while ( (current_time - initial_time) < time_limit ) {

        if ((iteration_ctr> (total_2opt_iterations/2) ) && (!saved_mid_path)) {
            saved_mid_path = true;
            for (int i=0; i<path.size(); i++) {
                save_path_2.push_back(path[i]);
            }
        }
        iteration_ctr++;
        bool no_update = true;
        // Single step: updating 1 iteration
        for (int i=0; i<path.size()-3; i++) {
            for (int j=i+2; j<path.size()-1; j++) {
                double ab = find_distance(points[path[i]].first, points[path[i]].second, points[path[i+1]].first, points[path[i+1]].second);
                double cd = find_distance(points[path[j]].first, points[path[j]].second, points[path[j+1]].first, points[path[j+1]].second);
                double ac = find_distance(points[path[i]].first, points[path[i]].second, points[path[j]].first, points[path[j]].second);
                double bd = find_distance(points[path[i+1]].first, points[path[i+1]].second, points[path[j+1]].first, points[path[j+1]].second);
                double difference = (ac+bd) - (ab+cd);
                if (difference<0) {
                    no_update = false;
                    path_length += difference;
                    reverse_vector_segment(path, i+1, j);
                    two_opt_edge_swaps++;
                }
            }
        }

        if (no_update) {
            break;
        }
        current_time = get_current_time();
    }
    total_2opt_iterations = iteration_ctr;
    return path;
}


bool test_tsp_path(vector<int> &path, int n) {
    if (path.size() != n+1) { 
        // The round trip does not have all nodes or have repeating nodes.
        return false;
    }

    if (path[0] != path[n]) {
        // The round trip is not a closed circuit
        return false;
    }

    set<int> covered_points;
    for (int i=0; i<n; i++) {
        if (covered_points.find(path[i]) != covered_points.end()) {
            // Path has repeating points
            return false;
        }
    }

    return true;
}


void set_2opt_iterations(vector<pair<float, float> > &points) {
    total_2opt_iterations = 0;
    vector<int> v1;
    vector<int> v2;
    vector<int> v3;
    two_opt_tsp(points, v1, v2, v3);
}

void reverse_vector_segment(vector<int> &path, int start, int end) {
    int mid = (start+end)/2;
    for (int i=start, j=end; i<=mid && j>=mid; i++, j--) {
        int temp = path[i];
        path[i] = path[j];
        path[j] = temp;
    }
}


double find_path_length(vector<pair<float, float> > points, vector<int> path) {
    double path_length = 0;
    for (int i=0; i<path.size(); i++) {
        // Finding distance between point p (i th) and q (i+1 th or 0 th for last i)
        int p = path[i];
        int q = path[(i+1)%path.size()];
        path_length += find_distance(points[p].first, points[p].second, points[q].first, points[q].second);
    }
    return path_length;
}


float find_distance(float x1, float y1, float x2, float y2) {
    float a = (x1-x2)*(x1-x2);
    float b = (y1-y2)*(y1-y2);
    return sqrt(a+b);
}


void print_path(vector<int> path) {
    for (int i=0; i<path.size(); i++) {
        cout<<path[i];
        if (i<path.size()-1) {
            cout<<" -> ";
        }
    }
    cout<<endl;
}


void write_path_to_file(vector<int> &path, vector<pair<float, float> > &points, string file_name) {
    ofstream output_file(BASE_PATH_OUTPUT + file_name);
    for (int i=0; i<path.size()-1; i++) {
        int p1 = path[i];
        int p2 = path[i+1];
        output_file<<p1<<" "<<p2<<" "<<find_distance(points[p1].first, points[p1].second, points[p2].first, points[p2].second)<<"\n";
    }
}


long long get_current_time() {
    auto currentTime = std::chrono::system_clock::now();
    auto duration = currentTime.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}


string get_file_name_without_extension(const string &file_path) {
    // Find the last occurrence of the dot (.) in the file path
    size_t dot_pos = file_path.find_last_of('.');

    // If a dot is found and it is not the first character, extract the substring before it
    if (dot_pos != std::string::npos && dot_pos != 0) {
        return file_path.substr(0, dot_pos);
    }

    // If no dot is found or it is the first character, return the entire file path
    return file_path;
}
