#pragma once

#include "table.hpp"

// Degree of the b+ tree
const unsigned int CHUNK_SIZE = 10;
 
class Chunk {
  private:

    bool is_leaf;
    unsigned int index;

    double data[CHUNK_SIZE];
    Chunk* pointers[CHUNK_SIZE + 1];

  public:
    Chunk(): index(0), is_leaf(false) {};
    friend class Table;
};