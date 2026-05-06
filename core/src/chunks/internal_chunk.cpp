#include "iostream"

#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

InternalChunk::InternalChunk() {
  this->num_filled = 0;

  // reserve allocates space in memory (does not default construct)
  this->keys.resize(Chunk::MAX_DEGREE + 1, std::numeric_limits<Key>::max());
}

// actions

InsertStatus InternalChunk::insert(Key key, [[maybe_unused]] const std::vector<Value>& val) {
  return this->insertKey(key).status;
}

void InternalChunk::insertChild(Chunk* chunk) {
  this->children.push_back(chunk);
}

InsertStatus InternalChunk::insertChild(Key key, Chunk* right_child) {
  // inserts child in correct sorted place in chunk
  unsigned int i = 0;
  while ((i < this->num_filled) && (key >= this->keys[i])) {
    i++;
  }

  for (unsigned int j = this->num_filled; j > i; j--) {
    this->keys[j] = this->keys[j - 1];
  }
  this->keys[i] = key;
  this->num_filled += 1;

  this->children.insert(this->children.begin() + i + 1, right_child);
  return this->num_filled > Chunk::MAX_DEGREE ? Full : Success;
}

SplitChunk InternalChunk::split() {
  // splits chunk across middle and returns pointers
  const unsigned int num_full = this->num_filled;
  const unsigned int middle = std::floor(this->num_filled / 2.0f);
  InternalChunk* new_chunk = new InternalChunk();
  const Key split_key = this->keys[middle];

  for (unsigned int i = middle + 1; i < num_full; i++) {
    // insert keys to right of the promoted key into the new chunk
    new_chunk->insertKey(this->keys[i]);
  }

  new_chunk->children.insert(
    new_chunk->children.end(),
    this->children.begin() + middle + 1,
    this->children.end()
  );

  this->num_filled = middle;
  (this->keys).erase((this->keys).begin() + middle, (this->keys).end());
  this->keys.resize(Chunk::MAX_DEGREE + 1, std::numeric_limits<Key>::max());
  this->children.erase(this->children.begin() + middle + 1, this->children.end());

  return {
    split_key,
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
    curr = curr->getChildChunk(key);
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
