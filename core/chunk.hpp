#pragma once

#include "table.hpp"

// Degree of the b+ tree
const unsigned int CHUNK_SIZE = 10;
 
class Chunk {
  private:
    unsigned int index;
    double data[CHUNK_SIZE];

  public:
    Chunk(): index(0) {};
    friend class Table;
};