#include <argparse/argparse.hpp>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <tuple>

#include "../include/prj/bst_vc.h"
#include "../include/prj/graph.h"
#include "../include/prj/utils.h"

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("BSTVC");

  program.add_argument("-gr", "--graph-input")
      .required()
      .help("Path for the graph file input");
  program.add_argument("-vc", "--vertex-cover-out")
      .required()
      .help("Path for the vertex cover output");
  program.add_argument("-k")
      .required()
      .help("Path for the vertex cover output")
      .scan<'i', int>();
  ;
  program.add_argument("-pg", "--print_graph").flag().help("Prints read graph");
  program.add_argument("-pvc", "--print-vertex-cover")
      .flag()
      .help("Print calculated vertex cover");

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  std::string graph_path = program.get("-gr");
  std::string vc_path = program.get("-vc");
  bool print_graph = program["-pg"] == true;
  bool print_vc = program["-pvc"] == true;
  int k = program.get<int>("-k");

  if (!std::filesystem::exists(graph_path)) {
    std::cerr << "Graph file: `" << graph_path << "` does not exist."
              << std::endl;
    return 1;
  };

  Graph g = parse_file(graph_path);

  auto start_2 = std::chrono::high_resolution_clock::now();
  std::set<int> c = bounded_search_tree_vertex_cover(g, k);
  auto stop2 = std::chrono::high_resolution_clock::now();

  auto duration_2 =
      std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start_2);

  std::cout << "BST took " << duration_2.count() << " microseconds"
            << std::endl;

  if (print_graph) g.print();

  if(c.size() > 0) {
      std::cout << "Vertex Cover -----" << std::endl;
      std::cout << "Length: " << c.size() << " of " << g.vertices.size()
                << " vertices " << std::endl;
      if (print_vc) {
          for (auto cc : c) {
              std::cout << cc << " ";
          }
      }
      std::cout << std::endl;
  } else {
      std::cout << "No vertex cover found" << std::endl;
  }



  write_vertex_cover(vc_path, g.vertices.size(), c);
}