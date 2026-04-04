#include <iostream>
#include <cmath>

#include "chunk.hpp"

LeafChunk::LeafChunk(unsigned int num_attributes) {
  this->next = nullptr;
  this->num_attributes = num_attributes;
  // reserve allocates space in memory (does not default construct)
  this->keys.reserve(this->MAX_DEGREE);
  // resize default constructs the items
  this->values.resize(num_attributes);
  for (std::vector<double>& val: this->values) {
    val.reserve(this->MAX_DEGREE);
  }
}

bool LeafChunk::isLeaf() const {
  return true;
}

bool LeafChunk::isFull() const {
  // ensures that leaf can only have max_degree - 1 keys
  if (this->keys.size() < (this->MAX_DEGREE - 1)) {
    return false;
  }
  return true;
}

bool LeafChunk::insert(unsigned int key, std::vector<double> val) {
  if (this->isFull() == false) {
    // case where space exists
    this->keys.push_back(key);
    // inserts values into respective attributes
    for (unsigned int i = 0; i < this->MAX_DEGREE; i++) {
      this->values[i].push_back(val[i]);
    }
    return true;
  }
  return false; // indicates that splits needs to occur
}

std::pair<Chunk*, Chunk*> LeafChunk::split() {
  // splits chunk across middle and returns pointers

}



// InternalChunk();
// bool isLeaf() const override; // returns false