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

  // find leaf
  std::stack<Chunk*> path;
  path.push(this->root);

  while (path.top()->isLeaf() == false) {
    // traverse down tree and find leaf
    InternalChunk* top = static_cast<InternalChunk*>(path.top());
    ChunkRes res = top->getNext(key);
    
    if (res.valid == false) {
      // duplicate key
      return false;
    }

    path.push(res.chunk);
  }

  // insert into leaf
  Chunk* current = path.top();
  LeafChunk* leaf = static_cast<LeafChunk*>(current);
  InsertStatus status = leaf->insert(key, row);
  path.pop();

  if (status == Invalid) 
    return false; // duplicate
  if (status == Success) 
    return true; // no split needed

  // split internal nodes when full
  while (status == Full) {
    SplitChunk split = current->split();

    if (path.empty()) {
      // no root
      InternalChunk* new_root = new InternalChunk();
      new_root->insert(key);
      new_root->insertChild(current);
      new_root->insertChild(split.chunk);
      this->root = new_root;
      return this->insert(key, row);
    }

    // when parent internal node exists
    InternalChunk* parent = static_cast<InternalChunk*>(path.top());
    path.pop();
    status = parent->insertChild(split.key, split.chunk);
    current = parent;

    if (status == Success) {
      return this->insert(key, row);
    }

  }

  if (status == Invalid) {
    return false;
  }
  return true;
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
  os << "Table:\n";
  // visualize data
  Chunk* cur = table.root;
  while (cur->isLeaf() == false) {
    cur = static_cast<InternalChunk*>(cur)->getFirst();
  }

  LeafChunk* first = static_cast<LeafChunk*>(cur);
  while (first != nullptr) {
    os << *first;
    first = first->getNext();
  }

  return os;
}
