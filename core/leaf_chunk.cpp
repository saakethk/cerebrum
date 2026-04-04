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
  if (this->keys.size() < this->MAX_DEGREE) {
    return false;
  }
  return true;
}

std::pair<unsigned int, std::vector<double>> LeafChunk::getValues(unsigned int index) const {
  // gets all values associated with a index
  unsigned int key = this->keys[index];
  std::vector<double> vals;
  vals.reserve(this->num_attributes);
  for (const std::vector<double>& val: this->values) {
    vals.push_back(val[index]);
  }
  return {key, vals};
}

bool LeafChunk::insert(unsigned int key, const std::vector<double>& val) {
  // case where space exists
  this->keys.push_back(key);
  // inserts values into respective attributes
  for (unsigned int i = 0; i < this->num_attributes; i++) {
    this->values[i].push_back(val[i]);
  }
  if (this->isFull()) {
    return true; // indicates that splits needs to occur
  }
  return false;
}

std::pair<Chunk*, Chunk*> LeafChunk::split() {
  // splits chunk across middle and returns pointers
  const unsigned int middle = std::floor(this->MAX_DEGREE / 2);
  LeafChunk* new_chunk = new LeafChunk(this->num_attributes);

  for (unsigned int i = middle; i < this->MAX_DEGREE; i++) {
    // insert the middle and all to right to new chunk
    std::pair<unsigned int, std::vector<double>> value = this->getValues(i);
    new_chunk->insert(value.first, value.second);
  }

  // delete the ones from middle onward in this chunk (inclusive)
  (this->keys).erase((this->keys).begin() + middle, (this->keys).end());
  for (std::vector<double>& val: this->values) {
    val.erase(val.begin() + middle, val.end());
  }

  return {this, new_chunk};
}

LeafChunk::~LeafChunk() {
  // Not needed at moment
}

std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk) {
  os << "Leaf Chunk:\n";
  // Visualize data
  for (unsigned int i = 0; i < chunk.keys.size(); i++) {
    os << "Key(" << chunk.keys[i] << ") ";
    for (unsigned int j = 0; j < chunk.num_attributes; j++) {
      os << chunk.values[j][i] << " ";
    }
    os << "\n";
  }
  return os;
}