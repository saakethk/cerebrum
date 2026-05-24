
#include "table/table.hpp"

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
      InternalChunk* new_root = new InternalChunk(this->chunk_size);
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

  Chunk* current = this->root;

  while (current->isLeaf() == false) {
    // traverse down to find leaf
    InternalChunk* i_cur = static_cast<InternalChunk*>(current);
    current = i_cur->getNext(key);
  }

  // remove from leaf 
  LeafChunk* leaf = static_cast<LeafChunk*>(current);
  bool valid = leaf->remove(key);

  if (valid == false) {
    // remove failed
    return valid;
  }
  
  this->num_rows -= 1;
  return valid;
}
