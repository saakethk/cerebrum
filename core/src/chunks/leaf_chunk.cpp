#include <iostream>
#include <cmath>
#include <limits>

#include "chunks/leaf_chunk.hpp"

LeafChunk::LeafChunk(unsigned int num_attributes) {
  this->num_attributes = num_attributes;
  this->next = nullptr;
  this->previous = nullptr;

  this->values.resize(num_attributes);
  for (std::vector<Value>& val: this->values) {
    val.resize(CHUNK_SIZE, 0);
  }
}

void LeafChunk::insertAttributeValue(
  unsigned int index, unsigned int attr_index, Value val) {
  // insert value into attribute vector
  for (unsigned int j = this->num_filled; j > index; j--) {
    // shifts values
    this->values[attr_index][j] = this->values[attr_index][j - 1];
  }
  
  this->values[attr_index][index] = val; // insert value
}

void LeafChunk::insertValue(unsigned int key_index, 
  const std::vector<Value>& row) {
  // inserts value into all attribute vectors
  for (unsigned int i = 0; i < this->num_attributes; i++) {
    this->insertAttributeValue(key_index, i, row[i]);
  }
}

void LeafChunk::removeAttributeValue(unsigned int index, unsigned int attr_index) {
  // insert value into attribute vector
  for (unsigned int j = index; j < this->keys.size(); j++) {
    // shifts values
    this->values[attr_index][j] = this->values[attr_index][j + 1];
  }
}

void LeafChunk::removeValue(unsigned int key_index) {
  // inserts value into all attribute vectors
  for (unsigned int i = 0; i < this->num_attributes; i++) {
    this->removeAttributeValue(key_index, i);
  }
}

InsertStatus LeafChunk::insert(Key key, std::vector<Value>& row) {
  if (this->isFull() == true) {
    // checks if full
    return Full;
  }

  // finds loc to insert
  KeyLoc loc = this->searchKey(key);
  if (loc.valid == true) {
    // key already exists
    return Invalid;
  }

  // insertion successful
  this->insertKey(loc.index, key);
  this->insertValue(loc.index, row);
  return Success;
}

bool LeafChunk::remove(Key key) {
  // finds key
  KeyLoc loc = this->searchKey(key);
  if (loc.valid == false) {
    // key not found
    return false;
  }

  // remove successful
  this->removeKey(loc.index);
  this->removeValue(loc.index);
  return true;
}

SplitChunk LeafChunk::split() {
  // splits chunk across middle and returns pointers
  const unsigned int num_full = this->num_filled;
  const unsigned int middle = std::floor(this->num_filled / 2.0f);
  LeafChunk* new_chunk = new LeafChunk(this->num_attributes);
  const Key split_key = this->keys[middle];

  for (unsigned int i = middle; i < num_full; i++) {
    // insert the middle and all to right to new chunk
    new_chunk->insert(this->keys[i], this->getRowByIndex(i));
  }

  // delete the ones from middle onward in this chunk (inclusive)
  (this->keys).erase((this->keys).begin() + middle, (this->keys).end());
  this->keys.resize(CHUNK_SIZE, 0);

  for (std::vector<Value>& val: this->values) {
    // erase attribute values
    val.erase(val.begin() + middle, val.end());
    val.resize(CHUNK_SIZE, 0);
  }

  // set links between split nodes
  this->num_filled = middle;
  this->next = new_chunk;
  new_chunk->previous = this;

  return {split_key, new_chunk};
}

bool LeafChunk::isLeaf() const {
  // leaf node
  return true;
}

unsigned int LeafChunk::getNumAttributes() const {
  return this->num_attributes;
}

LeafChunk* LeafChunk::getNext() {
  return this->next;
}

LeafChunk* LeafChunk::getPrevious() {
  return this->previous;
}

std::vector<Value>& LeafChunk::getRowByIndex(unsigned int index) {
  // gets row of values
  std::vector<Value> row;
  for (unsigned int i = 0; i < this->num_attributes; i++) {
    row.push_back(this->getRowVal(index, i));
  }
  return row;
}

Value LeafChunk::getRowVal(unsigned int index, unsigned int attr_index) {
  // gets singular value in row
  return this->values[attr_index][index];
}

RowResult LeafChunk::getRow(Key key) {
  // finds key
  KeyLoc loc = this->searchKey(key);
  if (loc.valid == false) {
    // key not found
    return {false, {}};
  }

  // gets row of values
  return {true, this->getRowByIndex(loc.index)};
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

// LeafChunk::LeafChunk(unsigned int num_attributes) {
  
//   this->next = nullptr;
//   this->num_attributes = num_attributes;
//   this->num_filled = 0;

//   // reserve allocates space in memory (does not default construct)
//   this->keys.resize(Chunk::MAX_DEGREE + 1, std::numeric_limits<Key>::max());

//   // resize default constructs the items
//   this->values.resize(num_attributes);
//   for (std::vector<double>& val: this->values) {
//     val.resize(Chunk::MAX_DEGREE + 1, std::numeric_limits<Value>::max()); // fills with max value of double
//   }

// }

// // actions

// SplitChunk LeafChunk::split() {
//   // splits chunk across middle and returns pointers
//   const unsigned int num_full = this->num_filled;
//   const unsigned int middle = std::floor(this->num_filled / 2.0f);
//   LeafChunk* new_chunk = new LeafChunk(this->num_attributes);
//   const Key split_key = this->keys[middle];

//   for (unsigned int i = middle; i < num_full; i++) {
//     // insert the middle and all to right to new chunk
//     new_chunk->insert(this->keys[i], this->getRow(i));
//   }

//   // delete the ones from middle onward in this chunk (inclusive)
//   (this->keys).erase((this->keys).begin() + middle, (this->keys).end());
//   this->keys.resize(Chunk::MAX_DEGREE + 1, std::numeric_limits<Key>::max());
//   for (std::vector<double>& val: this->values) {
//     val.erase(val.begin() + middle, val.end());
//     val.resize(Chunk::MAX_DEGREE + 1, std::numeric_limits<Value>::max());
//   }
//   new_chunk->next = this->next;
//   this->num_filled = middle;
//   this->next = new_chunk;
//   return {
//     split_key,
//     this, new_chunk};
// }

// // accessors

// bool LeafChunk::isLeaf() const {
//   return true;
// }

// LeafChunk* LeafChunk::getNext() {
//   return this->next;
// }

// std::pair<unsigned int, unsigned int> LeafChunk::size() const {
//   // return dimensions (num keys x num attributes)
//   std::pair<unsigned int, unsigned int> dims;
//   dims.first = this->num_filled;
//   dims.second = this->num_attributes;
//   return dims;
// }

// double LeafChunk::get(unsigned int index, unsigned int attribute_index) const {
//   // returns a specific attribute
//   return this->values[attribute_index][index];
// }

// std::vector<double> LeafChunk::getRow(unsigned int index) const {
//   // gets all values associated with a index
//   std::vector<double> vals;
//   vals.reserve(this->num_attributes);
//   for (unsigned int attribute_index = 0; attribute_index < this->num_attributes; attribute_index++) {
//     vals.push_back(this->get(index, attribute_index));
//   }
//   return vals;
// }

// LeafChunk::~LeafChunk() {
//   // not needed at moment
// }

// std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk) {
//   // visualize data
//   for (unsigned int i = 0; i < chunk.num_filled; i++) {
//     os << "Key(" << chunk.keys[i] << ") ";
//     for (unsigned int j = 0; j < chunk.num_attributes; j++) {
//       os << chunk.values[j][i] << " ";
//     }
//     os << "\n";
//   }
//   return os;
// }
