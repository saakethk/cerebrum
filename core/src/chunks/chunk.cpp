#include "chunks/chunk.hpp"

Chunk::Chunk() {
  this->num_filled = 0;
  this->keys.resize(CHUNK_SIZE, 0); // does not construct
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

bool Chunk::insertKey(unsigned int index, Key key) {
  // inserts key at provided index
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
  return Success;
}

void Chunk::removeKey(unsigned int index) {
  // removes key at provided index
  for (unsigned int j = index; j < this->keys.size(); j--) {
    // shifts values
    this->keys[j] = this->keys[j + 1];
  }
  this->num_filled -= 1;
}

bool Chunk::isFull() const {
  // ensures chunk size constraint satisfied
  if (this->num_filled < CHUNK_SIZE) {
    return false;
  }
  return true;
}

std::vector<Key>& Chunk::getKeys() {
  return this->keys;
}

unsigned int Chunk::getNumItems() const {
  return this->num_filled;
}

// Chunk* Chunk::getFirstChild() {
//   return this->children[0];
// }

// Chunk* Chunk::getChildChunk(Key key) {
//   // TODO: potentially implement binary search here
//   unsigned int i = 0;
//   while ((i < this->num_filled) && (key >= this->keys[i])) {
//     i++;
//   }
//   return this->children[i];
// }
