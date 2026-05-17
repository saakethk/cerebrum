#include <queue>
#include <iostream>
#include <array>
#include <sstream>
#include <string>
#include <regex>

#include "table.hpp"
#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

Table::Table(std::vector<Attribute> attributes) {
  // attribute size has to be known at inxitalization time
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
    val = this->evalEquation(key, this->virtual_attr_map[attribute]);
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
    row.push_back(this->evalEquation(key, pair.second));
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

        // get key val for index
        Key key = cur->getKeys()[i];

        for (auto& pair: this->virtual_attr_map) {
          // add virtual vals
          row.push_back(this->evalEquation(key, pair.second));
        }
        return {true, row};
      }
      j++;
    }
    
    cur = cur->getNext();
  }
  
  return {false, {}};
}

Value Table::evalEquation(Key key, std::string equation) {

  // assumes equation is validated
  std::stringstream e(equation);
  std::queue<std::string> vals;

  std::string val;
  while (e >> val) {
    // populates queue for equation
    vals.push(val);
  }

  Value res = 0;
  Operation op = ADD;
  while (vals.empty() == false) {
    // parses equation from left to right

    if (vals.front() == "(") {
      // find value of paranthesis
      vals.pop();

      unsigned int depth = 1;
      std::string sub_equation;
      while ((vals.empty() == false) && (depth > 0)) {
        std::string token = vals.front();
        vals.pop();

        if (token == "(") {
          depth++;
        } else if (token == ")") {
          depth--;
        }

        if (depth > 0) {
          if (sub_equation.empty() == false) {
            sub_equation += " ";
          }
          sub_equation += token;
        }
      }

      // evaluate the sub equation
      Value sub_res = this->evalEquation(key, sub_equation);
      res = this->evalOperation(res, op, sub_res);
      
    } else if (this->isConstant(vals.front()) == true) {

      // if constant
      Value constant = std::stof(vals.front());
      res = this->evalOperation(res, op, constant);
      vals.pop();

    } else if (this->isOperator(vals.front()) != INVALID) {
      
      // if operator
      op = this->isOperator(vals.front());
      vals.pop();

    } else if (
      (this->isAttribute(vals.front()) == true)
      || (this->isVirtualAttribute(vals.front()) == true)
    ) {
      
      // if attribute
      Value attr_val = this->getVal(key, vals.front()).val;
      res = this->evalOperation(res, op, attr_val);
      vals.pop();

    } else if (vals.front() == ")") {
      vals.pop();
    }
  }
  return res;
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
  // TODO: make uniform spacing
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
