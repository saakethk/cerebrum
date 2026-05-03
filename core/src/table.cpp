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
      (path.back())->getNextChunk(key)
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
    Chunk* top = path.back();
    path.pop_back();

    if (path.size() == 0) {
      // if theres no parent node
      InternalChunk* new_root = new InternalChunk();
      new_root->insertKey(split_chunk.key);
      new_root->insertChild(split_chunk.left);
      new_root->insertChild(split_chunk.right);
      this->root = new_root;
    } else {
      // if there is a parent node
      InternalChunk* parent = static_cast<InternalChunk*>(top);
      parent->insertKey(split_chunk.key);
      parent->insertChild(split_chunk.left);
      parent->insertChild(split_chunk.right);
    }

    // push back till leaf is reached
    cur = (path.back())->insert(key, row);
  }
  return true;
}

// accessors

Table::~Table() {
  // not needed at moment
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
  os << "Leaf Chunk:\n";
  // visualize data
  for (unsigned int i = 0; i < chunk.num_filled; i++) {
    os << "Key(" << chunk.keys[i] << ") ";
    for (unsigned int j = 0; j < chunk.num_attributes; j++) {
      os << chunk.values[j][i] << " ";
    }
    os << "\n";
  }
  return os;
}


