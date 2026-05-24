
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

ChunkLoc Table::getChunkOffset(LeafChunk* chunk, Index loc_index, int offset) {
  // get value at a specific offset from a chunk and its local index

  LeafChunk* cur = chunk;
  Index index = loc_index;
  int cur_offset = 0;

  while (cur_offset != offset) {
    // move till offset reached

    if (offset < cur_offset) {

      // move backwards
      cur_offset -= 1;

      if (index == 0) {
        // end of chunk reached
        cur = cur->getPrevious();

        if (cur == nullptr) {
          // offset is out of bounds
          return {false, cur, loc_index};
        }

        index = cur->getNumItems() - 1;
      } else {
        // to prevent index overflow
        index -= 1;
      }

    } else if (offset > cur_offset) {

      // move forwards
      index += 1;
      cur_offset += 1;

      if (index == cur->getNumItems()) {
        // end of chunk reached
        cur = cur->getNext();

        if (cur == nullptr) {
          // offset is out of bounds
          return {false, cur, loc_index};
        }

        index = 0;
      }

    }

  }
  return {true, cur, index};
}

ValResult Table::getVal(Key key, Attribute attribute, int offset) {

  bool exists_attr = isAttribute(attribute, this->attr_map);
  bool exists_virtual = isAttribute(attribute, this->virtual_attr_map);
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

  ChunkLoc found = this->getChunkOffset(leaf, loc.index, offset);
  if (found.valid == false) {
    // offset out of bounds
    return {false, 0};
  }

  return this->getValLocal(found.chunk, attribute, 
    found.offset, exists_virtual);
}

ValResult Table::getValLocal(LeafChunk* leaf, Attribute attr, unsigned int offset, bool is_virtual) {
  // gets local val in chunk

  Key key = leaf->getKeys()[offset];
  Value val;

  if (is_virtual== true) {

    // attribute exists in virtual map
    val = this->evalEquation(key, this->virtual_attr_map[attr]);

  } else {

    // attribute exists in normal map
    val = leaf->getRowValByIndex(offset, this->attr_map[attr]);

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
    unsigned int num_vals = cur->getNumItems();
    
    for (unsigned int i = 0; i < num_vals; i++) {
      if (j + i == index) {
        
        // updates cache
        this->last_accessed_index = j;
        this->last_accessed_chunk = cur;

        return this->getValLocal(cur, attribute, i, exists_virtual);

      }
    }
    
    if (index >= j + num_vals) {
      // index in next chunk
      cur = cur->getNext();
      j += num_vals;
    } else {
      // index in previous chunk
      cur = cur->getPrevious();
      j -= cur->getNumItems();
    }
  }
  
  return {false, 0};
}

RowResult Table::getRowLocal(LeafChunk* leaf, unsigned int offset) {
  // gets local row in chunk

  Key key = leaf->getKeys()[offset];
  std::vector<Value> row;
  row.resize(num_attributes);
  leaf->getRowByIndex(offset, row);

  for (auto& pair: this->virtual_attr_map) {
    // add virtual vals
    row.push_back(this->evalEquation(key, pair.second));
  }

  return {true, row};
}

RowResult Table::getRow(Key key, int offset) {

  // search leaf chunk
  LeafChunk* leaf = this->getLeaf(key);
  KeyLoc loc = leaf->searchKey(key);

  if (loc.valid == false) {
    // if key not found
    return {false, {}};
  }

  ChunkLoc found = this->getChunkOffset(leaf, loc.index, offset);
  if (found.valid == false) {
    // offset out of bounds
    return {false, {}};
  }

  return this->getRowLocal(found.chunk, found.offset);
}

RowResult Table::getRowIndex(Index index) {
  // gets row at a index starting from first

  if (index >= this->num_rows) {
    // index is out of bounds
    return {false, {}};
  }

  LeafChunk* cur = this->getFirst();
  unsigned int j = 0; // counts for valid indices

  while (cur != nullptr) {
    unsigned int num_vals = cur->getNumItems();
    
    for (unsigned int i = 0; i < num_vals; i++) {
      if (j + i == index) {

        // updates cache
        this->last_accessed_index = j;
        this->last_accessed_chunk = cur;

        return this->getRowLocal(cur, i);
      }
    }
    
    if (index >= j + num_vals) {
      // index in next chunk
      cur = cur->getNext();
      j += num_vals;
    } else {
      // index in previous chunk
      cur = cur->getPrevious();
      j -= cur->getNumItems();
    }
  }
  
  return {false, {}};
}
