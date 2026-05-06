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

// modifiers

bool Table::insert(Key key, std::vector<Value>& row) {

  // find leaf
  std::stack<Chunk*> path;
  path.push(this->root);

  while (!path.top()->isLeaf())
    path.push(path.top()->getChildChunk(key));

  // insert into leaf
  Chunk* current = path.top();
  path.pop();

  InsertStatus cur = current->insert(key, row);

  if (cur == Invalid) return false;  // duplicate or bad key
  if (cur == Success) return true;   // no split needed

  // split internal nodes when full
  while (cur == Full) {
    SplitChunk split = current->split();

    if (path.empty()) {
      // no root
      InternalChunk* new_root = new InternalChunk();
      new_root->insertKey(split.key);
      new_root->insertChild(split.left);
      new_root->insertChild(split.right);
      this->root = new_root;
      return this->insert(key, row);
    }

    // when parent internal node exists
    InternalChunk* parent = static_cast<InternalChunk*>(path.top());
    path.pop();
    cur = parent->insertChild(split.key, split.right);
    current = parent;

    if (cur == Success) {
      return this->insert(key, row);
    }

  }

  if (cur == Invalid) {
    return false;
  }
  return true;
}

// accessors

Table::~Table() {
  // not needed at moment
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
  os << "Table:\n";
  // visualize data
  Chunk* cur = table.root;
  while (cur->isLeaf() == false) {
    cur = cur->getFirstChild();
  }

  LeafChunk* first = static_cast<LeafChunk*>(cur);
  while (first != nullptr) {
    os << *first;
    first = first->getNext();
  }

  return os;
}
