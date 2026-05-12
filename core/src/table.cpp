#include <stack>
#include <iostream>
#include <array>

#include "table.hpp"
#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

Table::Table(std::vector<Attribute> attributes) {
  // attribute size has to be known at initalization time
  this->root = new LeafChunk(attributes.size());

  for (Attribute attr: attributes) {
    // initalizes mapping from attribute to index
    this->attr_map[attr] = this->num_attributes;
    this->num_attributes += 1;
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
    InternalChunk* i_cur = static_cast<InternalChunk*>(cur);
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

  Value val = leaf->getRowValByIndex(loc.index, this->attr_map.at(attribute));
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
  
  unsigned int j = 0; // counts for valid indices
  while (cur != nullptr) {
    unsigned int num_vals = cur->getNumVals();
    
    for (unsigned int i = 0; i < num_vals; i++) {
      if (j == index) {
        Value val = cur->getRowValByIndex(i, this->attr_map.at(attribute));
        return {true, val};
      }
      j++;
    }
    
    cur = cur->getNext();
  }
  
  return {false, 0};
}

RowResult Table::getRowIndex(Index index) const {
  // gets row at a index starting from first
  LeafChunk* cur = this->getFirst();
  
  unsigned int j = 0; // counts for valid indices
  while (cur != nullptr) {
    unsigned int num_vals = cur->getNumVals();
    
    for (unsigned int i = 0; i < num_vals; i++) {
      if (j == index) {
        std::vector<Value> val = cur->getRowByIndex(i);
        return {true, val};
      }
      j++;
    }
    
    cur = cur->getNext();
  }
  
  return {false, {}};
}

ValResult Table::parseOperation(std::string attribute_1, Operation op, std::string attribute_2, std::vector<Value> &row) {

  if (this->attr_map.find(attribute_1) == this->attr_map.end()) {
    // attribute 1 doesn't exist
    return {false, 0};
  } 
  
  if (this->attr_map.find(attribute_2) == this->attr_map.end()) {
    // attribute 2 doesn't exist
    return {false, 0};
  }

  Value first = row[this->attr_map[attribute_1]];
  Value second = row[this->attr_map[attribute_2]];

  // performs operation
  switch (op) {
    case ADD:
      return {true, first + second};
    case MULTIPLY:
      return {true, first * second};
    case DIVIDE:
      return {true, first / second};
    case EXPONENT:
      // TODO: write exponent function
      return {false, 0};
    case SUBTRACT:
      return {true, first - second};
  }

  return {false, 0};
}

Value parseEquation(std::string equation, std::vector<Value> &row) {
  // parses whole equation according to PEMDAS

  // checks attribute exists
}

bool Table::addAttribute(std::string name, std::string equation) {

  // adds the attribute to attr_map
  this->attr_map[name] = this->num_attributes;

  // adds the attribute to virtual_attr_map
  this->virtual_attr_map[name] = equation;

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

void Table::print() const {
  // for debugging
  LeafChunk* cur = this->getFirst();
  
  while (cur != nullptr) {
    unsigned int num_vals = cur->getNumVals();
    
    for (unsigned int i = 0; i < num_vals; i++) {
      std::vector<Value> val = cur->getRowByIndex(i);
      for (Value val: val) {
        std::cout << val << " ";
      }
      std::cout << std::endl;
    }
    
    cur = cur->getNext();
  }
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
