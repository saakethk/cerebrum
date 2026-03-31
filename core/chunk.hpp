#pragma once

#include "table.hpp"

// Degree of the b+ tree
const unsigned int CHUNK_SIZE = 10;

class Chunk {
  virtual bool isLeaf() const = 0;
  friend class Table;
};

class LeafChunk: Chunk {
  bool isLeaf() const; // returns true
  double keys[CHUNK_SIZE];
  Node* pointers[CHUNK_SIZE + 1];
};
 
class InternalChunk {
  private:
    unsigned int num_attributes;
    unsigned int filled_data;

    double keys[CHUNK_SIZE];
    Node* pointers[CHUNK_SIZE + 1];

  public:
    Chunk(unsigned int num_attributes): is_leaf(false), index(0), num_attributes(num_attributes), filled_data(0) {};
    bool insert(int row[]); // Return true if added, false if full
    std::string print(); // For visualization and debugging
    friend class Table;
};