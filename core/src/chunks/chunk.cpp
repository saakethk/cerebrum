#include "chunks/chunk.hpp"

bool Chunk::isFull() const {
  // ensures that leaf can only have max_degree - 1 keys
  if (this->num_filled < Chunk::MAX_DEGREE) {
    return false;
  }
  return true;
}

KeyLoc Chunk::searchKey(Key key) const {
  // binary search to find key loc
  unsigned int start = 0;
  unsigned int end = this->num_filled;
  unsigned int middle;
  while (start < end) {

    middle = (start + end) / 2;

    if (key > this->keys[middle]) {
      start = middle + 1;
    } else if (key < this->keys[middle]) {
      end = middle;
    } else if (key == this->keys[middle]) {
      // if key already exists
      return {false, middle};
    }
  }

  // Returns index to insert key into
  return {true, start};
}

Chunk* Chunk::getFirstChild() {
  return this->children[0];
}

Chunk* Chunk::getChildChunk(Key key) {
  // TODO: potentially implement binary search here
  unsigned int i = 0;
  while ((i < this->num_filled) && (key >= this->keys[i])) {
    i++;
  }
  return this->children[i];
}

InsertKeyStatus Chunk::insertKey(unsigned int key) {
  
  if (this->isFull()) {
    return {Full, 0};
  }

  unsigned int i = 0;
  while ((i < this->num_filled) && (key >= this->keys[i])) {
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
