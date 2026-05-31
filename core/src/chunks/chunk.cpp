#include "chunks/chunk.hpp"

Chunk::Chunk(unsigned int chunk_size): chunk_size(chunk_size), num_filled(0) {
  this->keys.resize(chunk_size, 0); // default constructs
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
      return {true, middle};
    }
  }

  // key doesnt exist; Returns index to insert key into
  return {false, start};
}

bool Chunk::insertKey(unsigned int index, Key key) {
  // inserts key at provided index

  KeyLoc loc = this->searchKey(key);
  if (loc.valid == true) {
    // key already exists
    return false;
  }

  if (this->isFull()) {
    return false;
  }

  for (unsigned int j = this->num_filled; j > index; j--) {
    // shifts values
    this->keys[j] = this->keys[j - 1];
  }

  // insert value
  this->keys[index] = key;
  this->num_filled += 1;
  return true;
}

void Chunk::removeKey(unsigned int index) {
  // removes key at provided index
  for (unsigned int j = index; j + 1 < this->num_filled; j++) {
    // shifts values
    this->keys[j] = this->keys[j + 1];
  }
  this->num_filled -= 1;
}

bool Chunk::exists(Key key) const {
  return (this->searchKey(key)).valid;
}

bool Chunk::isFull() const {
  // ensures chunk size constraint satisfied
  if (this->num_filled < chunk_size) {
    return false;
  }
  return true;
}

Key Chunk::getKey(unsigned int offset) const {

  if (offset > this->num_filled) {
    throw std::runtime_error("Offset is invalid for getKey");
  }

  return this->keys.at(offset);
}

unsigned int Chunk::getNumItems() const {
  return this->num_filled;
}
