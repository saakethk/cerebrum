#include <deque>
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

  std::deque<Chunk*> path;
  path.push_back(this->root);

  while ((path.back())->isLeaf() == false) {
    // push back till leaf is reached
    path.push_back(
      (path.back())->getChildChunk(key)
    );
  }

  InsertStatus cur = (path.back())->insert(key, row);
  while ((cur != Success) && (path.size() > 0)) {
    // find node to insert into
    if (cur == Invalid) {
      return false;
    }

    // splits when full
    SplitChunk split_chunk = (path.back())->split();
    path.pop_back();

    if (path.size() == 0) {
      // if theres no parent node
      InternalChunk* new_root = new InternalChunk();
      new_root->insertKey(split_chunk.key);
      new_root->insertChild(split_chunk.left);
      new_root->insertChild(split_chunk.right);
      this->root = new_root;
      return this->insert(key, row);
    } else {
      // if there is a parent node
      InternalChunk* parent = static_cast<InternalChunk*>(path.back());
      cur = parent->insertChild(split_chunk.key, split_chunk.right);
    }

    if (cur == Success) {
      return this->insert(key, row);
    }
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
    os << *first << std::endl;
    first = first->getNext();
  }

  return os;
}
