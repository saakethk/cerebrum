#include <iostream>
#include <cmath>
#include <limits>

#include "chunks/leaf_chunk.hpp"

LeafChunk::LeafChunk(unsigned int num_attributes) {
  
  this->next = nullptr;
  this->num_attributes = num_attributes;
  this->num_filled = 0;

  // reserve allocates space in memory (does not default construct)
  this->keys.resize(Chunk::MAX_DEGREE + 1, std::numeric_limits<Key>::max());

  // resize default constructs the items
  this->values.resize(num_attributes);
  for (std::vector<double>& val: this->values) {
    val.resize(Chunk::MAX_DEGREE + 1, std::numeric_limits<Value>::max()); // fills with max value of double
  }

}

// actions

void LeafChunk::insertAttributeValue(unsigned int index, double val, std::vector<double>& attribute_vals) {
  for (unsigned int j = this->num_filled; j > index; j--) {
    // shifts values
    attribute_vals[j] = attribute_vals[j - 1];
  }
  attribute_vals[index] = val; // insert value
}

void LeafChunk::insertValue(unsigned int key_index, const std::vector<double>& val) {
  for (unsigned int i = 0; i < this->num_attributes; i++) {
    this->insertAttributeValue(key_index, val[i], this->values[i]);
  }
}

InsertStatus LeafChunk::insert(unsigned int key, const std::vector<double>& val) {
  InsertKeyStatus key_insert = this->insertKey(key);

  if (key_insert.status == Success) {
    this->insertValue(key_insert.key, val);
  }
  
  return key_insert.status;
}

SplitChunk LeafChunk::split() {
  // splits chunk across middle and returns pointers
  const unsigned int num_full = this->num_filled;
  const unsigned int middle = std::floor(this->num_filled / 2.0f);
  LeafChunk* new_chunk = new LeafChunk(this->num_attributes);
  const Key split_key = this->keys[middle];

  for (unsigned int i = middle; i < num_full; i++) {
    // insert the middle and all to right to new chunk
    new_chunk->insert(this->keys[i], this->getRow(i));
  }

  // delete the ones from middle onward in this chunk (inclusive)
  (this->keys).erase((this->keys).begin() + middle, (this->keys).end());
  this->keys.resize(Chunk::MAX_DEGREE + 1, std::numeric_limits<Key>::max());
  for (std::vector<double>& val: this->values) {
    val.erase(val.begin() + middle, val.end());
    val.resize(Chunk::MAX_DEGREE + 1, std::numeric_limits<Value>::max());
  }
  new_chunk->next = this->next;
  this->num_filled = middle;
  this->next = new_chunk;
  return {
    split_key,
    this, new_chunk};
}

// accessors

bool LeafChunk::isLeaf() const {
  return true;
}

LeafChunk* LeafChunk::getNext() {
  return this->next;
}

std::pair<unsigned int, unsigned int> LeafChunk::size() const {
  // return dimensions (num keys x num attributes)
  std::pair<unsigned int, unsigned int> dims;
  dims.first = this->num_filled;
  dims.second = this->num_attributes;
  return dims;
}

double LeafChunk::get(unsigned int index, unsigned int attribute_index) const {
  // returns a specific attribute
  return this->values[attribute_index][index];
}

std::vector<double> LeafChunk::getRow(unsigned int index) const {
  // gets all values associated with a index
  std::vector<double> vals;
  vals.reserve(this->num_attributes);
  for (unsigned int attribute_index = 0; attribute_index < this->num_attributes; attribute_index++) {
    vals.push_back(this->get(index, attribute_index));
  }
  return vals;
}

LeafChunk::~LeafChunk() {
  // not needed at moment
}

std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk) {
  // visualize data
  for (unsigned int i = 0; i < chunk.num_filled; i++) {
    os << "Key(" << chunk.keys[i] << ") ";
    for (unsigned int j = 0; j < chunk.num_attributes; j++) {
      os << chunk.values[j][i] << " ";
    }
    os << "\n";
  }
  return os;
}
