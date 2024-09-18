#include "../include/prj/utils.h"

#include <fstream>
#include <iostream>

void write_vertex_cover(std::string& filename, int n_vertices,
                        std::set<int>& cover) {
  std::fstream vc_file;
  vc_file.open(filename, std::fstream::out);

  if (!vc_file.is_open()) {
    throw std::runtime_error("Could not open vertex cover output file");
  }

  if (cover.size() > 0) {
      vc_file << "s vc " << n_vertices << " " << cover.size() << std::endl;

      for (int cc : cover) {
          vc_file << cc << std::endl;
      }
  } else {
      vc_file << "No vertex cover found" << std::endl;
  }

}