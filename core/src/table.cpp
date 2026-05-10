#include <stack>
#include <iostream>
#include <array>

#include "table.hpp"
#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

Table::Table(std::vector<Attribute> attributes) {
  // attribute size has to be known at initalization time
  this->root = new LeafChunk(attributes.size());

  unsigned int i = 0;
  for (Attribute attr: attributes) {
    // initalizes mapping from attribute to index
    this->attr_map[attr] = 0;
    i += 1;
  }
}

LeafChunk* Table::getFirst() const {
  Chunk* cur = this->root;

  while (cur->isLeaf() == false) {
    // find smallest key leaf
    cur = static_cast<InternalChunk*>(cur)->getFirst();
  }
  return static_cast<LeafChunk*>(cur);
}

LeafChunk* Table::getLast() const {
  Chunk* cur = this->root;
  
  while (cur->isLeaf() == false) {
    // find largest key leaf
    cur = static_cast<InternalChunk*>(cur)->getLast();
  }
  return static_cast<LeafChunk*>(cur);
}

LeafChunk* Table::getLeaf(Key key) const {
  Chunk* cur = this->root;

  while (cur->isLeaf() == false) {
    // find specified key
    InternalChunk* i_cur = static_cast<InternalChunk*>(i_cur);
    cur = i_cur->getNext(key);
  }
  return static_cast<LeafChunk*>(cur);
}

ValResult Table::getVal(Key key, Attribute attribute) const {

  // check if attribute exists
  if ((this->attr_map).find(attribute) == this->attr_map.end()) {
    return {false, 0};
  }

  // search leaf chunk
  LeafChunk* leaf = this->getLeaf(key);
  KeyLoc loc = leaf->searchKey(key);

  if (loc.valid == false) {
    // if key not found
    return {false, 0};
  }

  Value val = leaf->getRowVal(loc.index, this->attr_map.at(attribute));
  return {true, val};
}

RowResult Table::getRow(Key key) const {

  // search leaf chunk
  LeafChunk* leaf = this->getLeaf(key);
  KeyLoc loc = leaf->searchKey(key);

  if (loc.valid == false) {
    // if key not found
    return {false, {}};
  }

  std::vector<Value> val = leaf->getRow(loc.index);
  return {true, val};
}

ValResult Table::getValIndex(Index index, Attribute attribute) const {
  // gets value at a index starting from first
  LeafChunk* cur = this->getFirst();

  unsigned int i = 0; // counts for all indices
  unsigned int j = 0; // counts for valid indices
  while (j < index) {
    unsigned int offset = (i % CHUNK_SIZE);
    if (offset < cur->getNumVals()) {
      cur->getRowValByIndex(index, this->attr_map.at(attribute));
    }

    i++;
  }

}

RowResult Table::getRowIndex(Index key) const {

}

bool Table::insert(Key key, std::vector<Value>& row) {

  // traverse down to find leaf
  std::stack<InternalChunk*> path;
  Chunk* current = this->root;
  while (current->isLeaf() == false) {
    path.push(static_cast<InternalChunk*>(current));
    current = static_cast<InternalChunk*>(current)->getNext(key);
  }

  // insert into leaf
  LeafChunk* leaf = static_cast<LeafChunk*>(current);
  InsertStatus status = leaf->insert(key, row);
  if (status == Invalid) {
    return false;
  } else if (status == Success) {
    this->num_rows += 1;
    return true;
  }

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

bool Table::remove(Key key) {

  std::stack<InternalChunk*> path;
  Chunk* current = this->root;

  while (current->isLeaf() == false) {
    // traverse down to find leaf
    InternalChunk* i_cur = static_cast<InternalChunk*>(current);
    path.push(i_cur);
    current = i_cur->getNext(key);
  }

  // remove from leaf 
  LeafChunk* leaf = static_cast<LeafChunk*>(current);
  bool valid = leaf->remove(key);

  if (valid == false) {
    // remove failed
    return valid;
  }
  
  while (path.empty() == false) {
    // remove from all internal chunks
    (path.top())->remove(key);
    path.pop();
  }
  this->num_rows -= 1;
  return valid;
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
  os << "Table:\n";

  LeafChunk* first = table.getLast();
  while (first != nullptr) {
    // starting from first and iterates through
    os << *first;
    first = first->getPrevious();
  }

  return os;
}
