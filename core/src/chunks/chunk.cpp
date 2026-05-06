#include "chunks/chunk.hpp"

bool Chunk::isFull() const {
  // ensures that leaf can only have max_degree - 1 keys
  if (this->num_filled < Chunk::MAX_DEGREE) {
    return false;
  }
  return true;
}

int Chunk::searchKey(Key key) const {
  // TODO: optimize this with binary search; currently basic linear search
  // for (unsigned int i = 0; i < this->num_filled; i++) {
  //   if (this->keys[i] == key) {
  //     return true;
  //   }
  // }

  // binary search to find key loc
  unsigned int start = 0;
  unsigned int end = this->num_filled;
  unsigned int middle = (start + end) / 2;
  while (start < end) {
    if (key > this->keys[middle]) {
      start = middle + 1;
    } else if (key < this->keys[middle]) {
      end = middle;
    } else if (key == this->keys[middle]) {
      return -1;
    }
  }

  // Returns index to insert key into
  return middle;
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
