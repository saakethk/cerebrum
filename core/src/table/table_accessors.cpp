
#include "table/table.hpp"

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
    InternalChunk* i_cur = static_cast<InternalChunk*>(cur);
    cur = i_cur->getNext(key);
  }
  return static_cast<LeafChunk*>(cur);
}

ValResult Table::getVal(Key key, Attribute attribute) {

  bool exists_attr = isAttribute(attribute, this->attr_map);
  bool exists_virtual = isVirtualAttribute(attribute, this->virtual_attr_map);
  if ((exists_attr == false) && (exists_virtual == false)) {
    // attribute doesn't exist
    return {false, 0};
  }

  // search leaf chunk
  LeafChunk* leaf = this->getLeaf(key);
  KeyLoc loc = leaf->searchKey(key);

  if (loc.valid == false) {
    // if key not found
    return {false, 0};
  }

  Value val;
  if (exists_virtual == true) {
    // attribute exists in virtual map
    std::vector<Value> row = leaf->getRow(loc.index);
    val = this->evalEquation(key, this->virtual_attr_map[attribute]);
  } else {
    // attribute exists in normal map
    val = leaf->getRowValByIndex(loc.index, this->attr_map[attribute]);
  }

  return {true, val};
}

ValResult Table::getValIndex(Index index, Attribute attribute) {
  // gets value at a index starting from first

  bool exists_attr = this->attr_map.find(attribute) != this->attr_map.end();
  bool exists_virtual = this->virtual_attr_map.find(attribute) != this->virtual_attr_map.end();
  if ((exists_attr == false) && (exists_virtual == false)) {
    // attribute doesn't exist
    return {false, 0};
  }

  if (index >= this->num_rows) {
    // index is out of bounds
    return {false, 0};
  }

  LeafChunk* cur = this->last_accessed_chunk;
  
  unsigned int j = this->last_accessed_index; // counts for valid indices
  while (cur != nullptr) {
    unsigned int num_vals = cur->getNumVals();
    
    for (unsigned int i = 0; i < num_vals; i++) {
      if (j + i == index) {
        
        // updates cache
        this->last_accessed_index = j;
        this->last_accessed_chunk = cur;

        // get key val for index
        Key key = cur->getKeys()[i];

        Value val;
        if (exists_virtual == true) {

          // attribute exists in virtual map
          std::vector<Value> row = cur->getRowByIndex(i);
          val = this->evalEquation(key, this->virtual_attr_map[attribute]);

        } else {

          // attribute exists in normal map
          val = cur->getRowValByIndex(i, this->attr_map[attribute]);

        }
        return {true, val};

      }
    }
    
    if (index > j) {
      cur = cur->getNext();
      j += num_vals;
    } else {
      cur = cur->getPrevious();
      j -= num_vals;
    }
  }
  
  return {false, 0};
}



RowResult Table::getRow(Key key) {

  // search leaf chunk
  LeafChunk* leaf = this->getLeaf(key);
  KeyLoc loc = leaf->searchKey(key);

  if (loc.valid == false) {
    // if key not found
    return {false, {}};
  }

  std::vector<Value> row = leaf->getRow(loc.index);

  for (auto& pair: this->virtual_attr_map) {
    // add virtual vals
    row.push_back(this->evalEquation(key, pair.second));
  }
  return {true, row};
}

RowResult Table::getRowIndex(Index index) {
  // gets row at a index starting from first
  LeafChunk* cur = this->getFirst();
  
  unsigned int j = 0; // counts for valid indices
  while (cur != nullptr) {
    unsigned int num_vals = cur->getNumVals();
    
    for (unsigned int i = 0; i < num_vals; i++) {
      if (j + i == index) {

        // updates cache
        this->last_accessed_index = j;
        this->last_accessed_chunk = cur;

        // get key and row
        Key key = cur->getKeys()[i];
        std::vector<Value> row = cur->getRowByIndex(i);

        for (auto& pair: this->virtual_attr_map) {
          // add virtual vals
          row.push_back(this->evalEquation(key, pair.second));
        }
        return {true, row};
      }
    }
    
    if (index > j) {
      cur = cur->getNext();
      j += num_vals;
    } else {
      cur = cur->getPrevious();
      j -= num_vals;
    }
  }
  
  return {false, {}};
}
