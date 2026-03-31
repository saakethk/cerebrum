#include <iostream>

#include "chunk.hpp"

bool Chunk::insert(int row[]) {
  // assumption that row is of size num_attributes
  for (unsigned int i = 0; i < this->num_attributes; i++) {
    std::cout << this->data[i] << std::endl;
  }
}

std::string Chunk::print() {
  // for visualization and debugging
  for (unsigned int i = 0; i < this->filled_data; i++) {
    std::cout << i << " ";
    if ((i != 0) && (i % this->num_attributes) == 0) {
      // print newline at end of every row
      std::cout << std::endl;
    }
  }
}