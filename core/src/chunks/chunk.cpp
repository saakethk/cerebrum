#include "chunks/chunk.hpp"

bool Chunk::isFull() const {
  // ensures that leaf can only have max_degree - 1 keys
  if (this->num_filled < Chunk::MAX_DEGREE) {
    return false;
  }
  return true;
}

unsigned int Chunk::searchKey(unsigned int key) const {
  // TODO: optimize this with binary search; currently basic linear search
  for (unsigned int i = 0; i < this->num_filled; i++) {
    if (this->keys[i] == key) {
      return i;
    }
  }

  // Return maximum value of unsigned int if not found
  return std::numeric_limits<unsigned int>::max();
}

Chunk* Chunk::getNextChunk(unsigned int key) {
  // TODO: potentially implement binary search here
  unsigned int i = 0;
  while (key > this->keys[i] && i < this->children.size()) {
    i++;
  }
  return this->children[i];
}

InsertKeyStatus Chunk::insertKey(unsigned int key) {
  
  if (this->isFull()) {
    return {Full, 0};
  }

  unsigned int i = 0;
  while ((i < Chunk::MAX_DEGREE) && (key >= this->keys[i])) {
    // finds correct part to insert into
    i++;
  }

  if ((i > 0) && (this->keys[i - 1] == key)) {
    // key already exists
    return {Invalid, 0};
  }

  for (unsigned int j = this->num_filled; j > i; j--) {
    // shifts values
    this->keys[j] = this->keys[j - 1];
  }
  this->keys[i] = key; // insert value
  this->num_filled += 1;
  return {Success, i};
}