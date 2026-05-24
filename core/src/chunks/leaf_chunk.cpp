#include <iostream>
#include <cmath>
#include <limits>

#include "chunks/leaf_chunk.hpp"

LeafChunk::LeafChunk(unsigned int chunk_size, unsigned int num_attributes): Chunk(chunk_size) {
  this->num_attributes = num_attributes;
  this->next = nullptr;
  this->previous = nullptr;

  this->values.resize(num_attributes);
  for (std::vector<Value>& val: this->values) {
    val.resize(chunk_size, 0);
  }
}

void LeafChunk::insertAttributeValue(Index index, Index attr_index, Value val) {
  // insert value into attribute vector
  for (unsigned int j = this->num_filled; j > index; j--) {
    // shifts values
    this->values[attr_index][j] = this->values[attr_index][j - 1];
  }
  
  this->values[attr_index][index] = val; // insert value
}

void LeafChunk::insertValue(Index key_index, const std::vector<Value>& row) {
  // inserts value into all attribute vectors
  for (unsigned int i = 0; i < this->num_attributes; i++) {
    this->insertAttributeValue(key_index, i, row[i]);
  }
}

void LeafChunk::removeAttributeValue(Index index, Index attr_index) {
  // insert value into attribute vector
  for (unsigned int j = index; j + 1 < this->num_filled; j++) {
    // shifts values
    this->values[attr_index][j] = this->values[attr_index][j + 1];
  }
}

void LeafChunk::removeValue(Index key_index) {
  // inserts value into all attribute vectors
  for (unsigned int i = 0; i < this->num_attributes; i++) {
    this->removeAttributeValue(key_index, i);
  }
}

InsertStatus LeafChunk::insert(Key key, const std::vector<Value>& row) {
  // find key before checking fullness so duplicate keys are rejected even in a full chunk
  KeyLoc loc = this->searchKey(key);
  if (loc.valid == true) {
    // key already exists
    return Invalid;
  }

  if (this->isFull() == true) {
    // checks if full
    return Full;
  }

  // insertion successful
  this->insertValue(loc.index, row);
  this->insertKey(loc.index, key);
  return Success;
}

bool LeafChunk::remove(Key key) {
  // finds key
  KeyLoc loc = this->searchKey(key);
  if (loc.valid == false) {
    // key is not found
    return false;
  }

  // remove successful
  this->removeValue(loc.index);
  this->removeKey(loc.index);
  return true;
}

SplitChunk LeafChunk::split() {
  // splits chunk across middle and returns pointers
  const unsigned int num_full = this->num_filled;
  const unsigned int middle = std::floor(this->num_filled / 2.0f);
  LeafChunk* new_chunk = new LeafChunk(this->chunk_size, this->num_attributes);
  const Key split_key = this->keys[middle];

  std::vector<Value> res;
  res.resize(num_attributes);
  for (unsigned int i = middle; i < num_full; i++) {
    // insert the middle and all to right to new chunk
    this->getRowByIndex(i, res);
    new_chunk->insert(this->keys[i], res);
  }

  // delete the ones from middle onward in this chunk (inclusive)
  (this->keys).erase((this->keys).begin() + middle, (this->keys).end());
  this->keys.resize(this->chunk_size, 0);

  for (std::vector<Value>& val: this->values) {
    // erase attribute values
    val.erase(val.begin() + middle, val.end());
    val.resize(this->chunk_size, 0);
  }

  // set links between split nodes
  this->num_filled = middle;
  new_chunk->next = this->next;
  if (this->next != nullptr) {
      this->next->previous = new_chunk;
  }
  this->next = new_chunk;
  new_chunk->previous = this;

  return {split_key, new_chunk};
}

bool LeafChunk::isLeaf() const {
  // leaf node
  return true;
}

LeafChunk* LeafChunk::getNext() {
  return this->next;
}

LeafChunk* LeafChunk::getPrevious() {
  return this->previous;
}

void LeafChunk::getRowByIndex(Index index, std::vector<Value>& row) {
  // gets row of values

  for (unsigned int i = 0; i < this->num_attributes; i++) {
    // assumes that row is resized to attribute size
    row[i] = this->getRowValByIndex(index, i);
  }
}

Value LeafChunk::getRowValByIndex(Index index, Index attr_index) {
  // gets singular value in row
  return this->values[attr_index][index];
}

void LeafChunk::getRow(Key key, std::vector<Value>& row) {
  // gets row of values for key
  KeyLoc loc = this->searchKey(key);
  if (loc.valid == false) {
    throw std::runtime_error("Invalid key supplied to getRow");
  }

  this->getRowByIndex(loc.index, row);
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
