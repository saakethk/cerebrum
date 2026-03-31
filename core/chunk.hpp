#pragma once

#include "table.hpp"

// Degree of the b+ tree
const unsigned int CHUNK_SIZE = 10;
 
class Chunk {
  private:

    bool is_leaf;

    unsigned int index;
    unsigned int num_attributes;
    unsigned int filled_data;

    double data[CHUNK_SIZE];
    Chunk* pointers[CHUNK_SIZE + 1];

  public:
    Chunk(unsigned int num_attributes): is_leaf(false), index(0), num_attributes(num_attributes), filled_data(0) {};
    bool insert(int row[]); // Return true if added, false if full
    std::string print(); // For visualization and debugging
    friend class Table;
};