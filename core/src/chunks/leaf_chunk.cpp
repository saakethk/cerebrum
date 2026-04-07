#include <iostream>
#include <cmath>
#include <limits>

#include "chunks/leaf_chunk.hpp"

LeafChunk::LeafChunk(unsigned int num_attributes) {
  this->next = nullptr;
  this->num_attributes = num_attributes;
  this->num_filled = 0;
  // reserve allocates space in memory (does not default construct)
  this->keys.resize(Chunk::MAX_DEGREE, std::numeric_limits<double>::max());
  // resize default constructs the items
  this->values.resize(num_attributes);
  for (std::vector<double>& val: this->values) {
    val.resize(Chunk::MAX_DEGREE, std::numeric_limits<double>::max()); // fills with max value of double
  }
}

bool LeafChunk::isLeaf() const {
  return true;
}

bool LeafChunk::isFull() const {
  // ensures that leaf can only have max_degree - 1 keys
  if (this->num_filled < Chunk::MAX_DEGREE) {
    return false;
  }
  return true;
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

unsigned int LeafChunk::searchKey(unsigned int key) const {
  // TODO: optimize this with binary search; currently basic linear search
  for (unsigned int i = 0; i < this->num_filled; i++) {
    if (this->keys[i] == key) {
      return i;
    }
  }
  return std::numeric_limits<unsigned int>::max(); // Return maximum value of unsigned int if not found
}

unsigned int LeafChunk::insertKey(unsigned int key) {
  
  if (this->isFull()) {
    // Return maximum value of unsigned int if max size reached
    return std::numeric_limits<unsigned int>::max();
  }

  unsigned int i = 0;
  while ((i < Chunk::MAX_DEGREE) && (key >= this->keys[i])) {
    // finds correct part to insert into
    i++;
  }
  if ((i > 0) && (this->keys[i - 1] == key)) {
    // Return maximum value of unsigned int if key already exists
    return std::numeric_limits<unsigned int>::max();
  }

  for (unsigned int j = this->num_filled; j > i; j--) {
    // shifts values
    this->keys[j] = this->keys[j - 1];
  }
  this->keys[i] = key; // insert value
  return i;
}

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

const std::vector<unsigned int>& LeafChunk::getKeys() const {
  return this->keys;
}

bool LeafChunk::insert(unsigned int key, const std::vector<double>& val) {
  // case where space exists
  unsigned int key_index = this->insertKey(key);
  if (key_index == std::numeric_limits<unsigned int>::max()) {
    return false; // indicates that insert failed. Either full or key exists
  }
  // inserts values into respective attributes
  this->insertValue(key_index, val);
  this->num_filled += 1;
  return true;
}

std::pair<Chunk*, Chunk*> LeafChunk::split() {
  // splits chunk across middle and returns pointers
  const unsigned int num_full = this->num_filled;
  const unsigned int middle = std::floor(this->num_filled / 2.0f);
  LeafChunk* new_chunk = new LeafChunk(this->num_attributes);

  for (unsigned int i = middle; i < num_full; i++) {
    // insert the middle and all to right to new chunk
    new_chunk->insert(this->keys[i], this->getRow(i));
    this->num_filled -= 1;
  }

  // delete the ones from middle onward in this chunk (inclusive)
  (this->keys).erase((this->keys).begin() + middle, (this->keys).end());
  for (std::vector<double>& val: this->values) {
    val.erase(val.begin() + middle, val.end());
  }
  this->next = new_chunk;

  return {this, new_chunk};
}

LeafChunk* LeafChunk::getNext() {
  return this->next;
}

LeafChunk::~LeafChunk() {
  // not needed at moment
}

std::pair<unsigned int, unsigned int> LeafChunk::size() const {
  // return dimensions (num keys x num attributes)
  std::pair<unsigned int, unsigned int> dims;
  dims.first = this->num_filled;
  dims.second = this->num_attributes;
  return dims;
}

std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk) {
  os << "Leaf Chunk:\n";
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