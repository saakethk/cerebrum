#include "iostream"

#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

InternalChunk::InternalChunk(Chunk* left_chunk) {
  this->children.push_back(left_chunk);
}

Chunk* InternalChunk::findNextChunk(unsigned int key) {
  unsigned int i = 0;
  while (key > this->keys[i] && i < this->children.size()) {
    i++;
  }
  return this->children[i];
}

std::vector<double> InternalChunk::get(unsigned int key) {
  Chunk* curr = this;
  while (curr->isLeaf() == false) {
    // traverses the children till leaf node reached
    curr = static_cast<InternalChunk*>(curr)->findNextChunk(key);
  }
  return static_cast<LeafChunk*>(curr)->getRow(key);
} 

bool insert(unsigned int key, Chunk* children) {
  // TODO
}

bool isLeaf() {
  // returns false
  return false;
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