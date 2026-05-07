#include <stack>
#include <iostream>

#include "table.hpp"
#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

Table::Table(std::vector<std::string> attributes) {
  // attribute size has to be known at initalization time
  this->attributes = attributes;
  this->root = new LeafChunk(attributes.size());
}

bool Table::insert(Key key, std::vector<Value>& row) {

  // traverse down to find leaf
  std::stack<InternalChunk*> path;
  Chunk* current = this->root;
  while (!current->isLeaf()) {
    path.push(static_cast<InternalChunk*>(current));
    current = static_cast<InternalChunk*>(current)->getNext(key);
  }

  // insert into leaf
  LeafChunk* leaf = static_cast<LeafChunk*>(current);
  InsertStatus status = leaf->insert(key, row);
  if (status != Full) 
    return (status == Success);

  // when leaf is full
  SplitChunk split = leaf->split();
  while (true) {

    if (path.empty()) {
      // when root doesn't exist
      InternalChunk* new_root = new InternalChunk();
      new_root->insert(split.key);
      new_root->insertChild(this->root);
      new_root->insertChild(split.chunk);
      this->root = new_root;
      return this->insert(key, row);
    }

    // starts at parent of leaf node
    InternalChunk* parent = path.top(); 
    path.pop();
    if (parent->isFull() == false) {
      // if parent not full
      status = parent->insertChild(split.key, split.chunk);

      if (status == Success) 
        return this->insert(key, row);
      if (status == Invalid)
        return false;
    }

    // splits parent if full
    SplitChunk parent_split = parent->split();
    if (split.key < parent_split.key) {
      // inserts into left half
      parent->insertChild(split.key, split.chunk);
    } else {
      // inserts into right half
      InternalChunk* right = static_cast<InternalChunk*>(parent_split.chunk);
      right->insertChild(split.key, split.chunk);
    }
    split = parent_split;

  }
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
  os << "Table:\n";

  Chunk* cur = table.root;
  while (cur->isLeaf() == false) {
    // find smallest key leaf
    cur = static_cast<InternalChunk*>(cur)->getFirst();
  }

  LeafChunk* first = static_cast<LeafChunk*>(cur);
  while (first != nullptr) {
    // starting from first and iterates through
    os << *first;
    first = first->getNext();
  }

  return os;
}
