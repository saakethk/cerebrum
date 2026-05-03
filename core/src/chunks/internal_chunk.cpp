#include "iostream"

#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

// actions

InsertStatus InternalChunk::insert(Key key, const std::vector<Value>& val) {
  return this->insertKey(key).status;
}

InsertStatus InternalChunk::insertChild(Chunk* chunk) {
  this->children.push_back(chunk);
}

SplitChunk InternalChunk::split() {
  // splits chunk across middle and returns pointers
  const unsigned int num_full = this->num_filled;
  const unsigned int middle = std::floor(this->num_filled / 2.0f);
  InternalChunk* new_chunk = new InternalChunk();
  new_chunk->children.push_back(new_chunk);

  for (unsigned int i = middle; i < num_full; i++) {
    // insert the middle and all to right to new chunk
    new_chunk->insertKey(this->keys[i]);
    this->num_filled -= 1;
  }

  // delete the ones from middle onward in this chunk (inclusive)
  (this->keys).erase((this->keys).begin() + middle, (this->keys).end());
  return {
    this->keys[middle], 
    this, new_chunk};
}

// accessors

bool InternalChunk::isLeaf() const {
  return false;
}

std::vector<Value> InternalChunk::get(Key key) {
  Chunk* curr = this;
  while (curr->isLeaf() == false) {
    // traverses the children till leaf node reached
    curr = curr->getNextChunk(key);
  }
  return static_cast<LeafChunk*>(curr)->getRow(key);
} 

std::ostream& operator<<(std::ostream& os, const InternalChunk& chunk) {
  os << "Internal Chunk: ";
  // visualize data
  for (unsigned int key: chunk.keys) {
    os << "Key(" << key << ") ";
  }
  os << "\n";
  return os;
}

InternalChunk::~InternalChunk() {
  // not needed at moment
}