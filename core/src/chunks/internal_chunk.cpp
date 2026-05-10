#include "iostream"

#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

InternalChunk::InternalChunk() {
  this->children.reserve(CHUNK_SIZE + 1);
}

Chunk* InternalChunk::getNext(Key key) {
  // gets next chunk in tree
  unsigned int i = 0;
  while ((i < this->num_filled) && (key >= this->keys[i])) {
    i++;
  }
  return this->children[i];
}

Chunk* InternalChunk::getFirst() const {
  // return left-most child
  return this->children[0];
}

Chunk* InternalChunk::getLast() const {
  // return right-most child: internal nodes hold num_filled+1 children
  return this->children[this->num_filled];
}

InsertStatus InternalChunk::insert(Key key) {
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
  this->insertKey(loc.index, key);
  return Success;
}

void InternalChunk::insertChild(Chunk* chunk) {
  this->children.push_back(chunk);
}

InsertStatus InternalChunk::insertChild(Key key, Chunk* chunk) {
  KeyLoc loc = this->searchKey(key);
  if (loc.valid == true) {
    // key already exists
    return Invalid;
  }

  if (this->isFull() == true) {
    // checks if full
    return Full;
  }

  this->insertKey(loc.index, key);
  this->children.insert(this->children.begin() + loc.index + 1, chunk);
  return Success;
}

bool InternalChunk::remove(Key key) {

  // finds key
  KeyLoc loc = this->searchKey(key);
  if (loc.valid == false) {
    // key not found
    return false;
  }

  this->removeKey(loc.index);
  return true;
}

SplitChunk InternalChunk::split() {
  // splits chunk across middle and returns pointers
  const unsigned int num_full = this->num_filled;
  const unsigned int middle = std::floor(this->num_filled / 2.0f);
  InternalChunk* new_chunk = new InternalChunk();
  const Key split_key = this->keys[middle];

  for (unsigned int i = middle + 1; i < num_full; i++) {
    // insert keys to right of the promoted key into the new chunk
    new_chunk->insertKey(new_chunk->num_filled, this->keys[i]);
  }

  new_chunk->children.insert(
    new_chunk->children.end(),
    this->children.begin() + middle + 1,
    this->children.end()
  );

  this->num_filled = middle;
  (this->keys).erase((this->keys).begin() + middle, (this->keys).end());
  this->keys.resize(CHUNK_SIZE, 0);
  this->children.erase(this->children.begin() + middle + 1, this->children.end());

  return {split_key, new_chunk};
}

bool InternalChunk::isLeaf() const {
  // internal chunk
  return false;
}

std::ostream& operator<<(std::ostream& os, const InternalChunk& chunk) {
  // debug function
  os << "Internal Chunk: ";
  for (unsigned int key: chunk.keys) {
    os << "Key(" << key << ") ";
  }
  os << "\n";
  return os;
}
