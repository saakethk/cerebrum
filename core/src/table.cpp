#include <stack>
#include <iostream>
#include <array>

#include "table.hpp"
#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

Table::Table(std::vector<Attribute> attributes) {
  // attribute size has to be known at initalization time
  this->num_rows = 0;
  this->num_attributes = 0;
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

ValResult Table::getVal(Key key, Attribute attribute) {

  bool exists_attr = this->attr_map.find(attribute) != this->attr_map.end();
  bool exists_virtual = this->virtual_attr_map.find(attribute) != this->virtual_attr_map.end();
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
    val = this->parseEquation(this->virtual_attr_map[attribute], row);
  } else {
    // attribute exists in normal map
    val = leaf->getRowValByIndex(loc.index, this->attr_map[attribute]);
  }

  return {true, val};
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
    row.push_back(this->parseEquation(pair.second, row));
  }
  return {true, row};
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

  LeafChunk* cur = this->getFirst();
  
  unsigned int j = 0; // counts for valid indices
  while (cur != nullptr) {
    unsigned int num_vals = cur->getNumVals();
    
    for (unsigned int i = 0; i < num_vals; i++) {
      if (j == index) {
        // correct index found

        Value val;
        if (exists_virtual == true) {
          // attribute exists in virtual map
          std::vector<Value> row = cur->getRowByIndex(i);
          val = this->parseEquation(this->virtual_attr_map[attribute], row);
        } else {
          // attribute exists in normal map
          val = cur->getRowValByIndex(i, this->attr_map[attribute]);
        }
        return {true, val};

      }
      j++;
    }
    
    cur = cur->getNext();
  }
  
  return {false, 0};
}

RowResult Table::getRowIndex(Index index) {
  // gets row at a index starting from first
  LeafChunk* cur = this->getFirst();
  
  unsigned int j = 0; // counts for valid indices
  while (cur != nullptr) {
    unsigned int num_vals = cur->getNumVals();
    
    for (unsigned int i = 0; i < num_vals; i++) {
      if (j == index) {
        std::vector<Value> row = cur->getRowByIndex(i);

        for (auto& pair: this->virtual_attr_map) {
          // add virtual vals
          row.push_back(this->parseEquation(pair.second, row));
        }
        return {true, row};
      }
      j++;
    }
    
    cur = cur->getNext();
  }
  
  return {false, {}};
}

Value Table::parseOperation(std::string attribute_1, Operation op, std::string attribute_2, std::vector<Value> &row) {

  if (this->attr_map.find(attribute_1) == this->attr_map.end()) {
    // attribute 1 doesn't exist
    return -1;
  } 
  
  if (this->attr_map.find(attribute_2) == this->attr_map.end()) {
    // attribute 2 doesn't exist
    return -1;
  }

  Value first = row[this->attr_map[attribute_1]];
  Value second = row[this->attr_map[attribute_2]];

  // performs operation
  switch (op) {
    case ADD:
      return first + second;
    case MULTIPLY:
      return first * second;
    case DIVIDE:
      return first / second;
    case EXPONENT:
      // TODO: write exponent function
      return 0;
    case SUBTRACT:
      return first - second;
  }

  // operation not found
  return -1;
}

Value Table::parseEquation(std::string equation, std::vector<Value> &row) {
  // TODO: implement this
  // parses whole equation according to PEMDAS
  // example
  // std::stack<std::string> vals;
  std::string test = equation;
  // checks attribute exists
  return row[0];
}

bool Table::addAttribute(std::string name, std::string equation) {

  if (this->attr_map.find(name) != this->attr_map.end()) {
    // name exists in attributes
    return false;
  }

  if (this->virtual_attr_map.find(name) != this->virtual_attr_map.end()) {
    // name exists in virtual attributes
    return false;
  }

  // TODO: validate equation

  // adds the attribute to virtual_attr_map
  this->virtual_attr_map[name] = equation;

  return true;
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

void Table::printHeaders() {
  // headers in order
  std::vector<Attribute> headers(this->num_attributes);
  for (auto& pair: this->attr_map) {
    headers[pair.second] = pair.first;
  }

  for (Attribute attr: headers) {
    // print normal attributes
    std::cout << attr << " ";
  }

  for (auto& pair: this->virtual_attr_map) {
    // print virtual attributes
    std::cout << pair.first << " ";
  }

  std::cout << std::endl;
}

void Table::printValues() {
  // values in order
  for (unsigned int i = 0; i < this->num_rows; i++) {
    for (Value res: this->getRowIndex(i).row) {
      std::cout << res << " ";
    }
    std::cout << std::endl;
  }
}

void Table::print() {
  this->printHeaders();
  this->printValues();
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
