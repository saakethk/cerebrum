#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "chunks/chunk.hpp"
#include "chunks/leaf_chunk.hpp"

using Attribute = std::string;

enum Operation {
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE
};

struct ValResult {
  bool valid;
  Value val;
};

struct RowResult {
  bool valid;
  std::vector<Value> row;
};

class Table {
  private:
  
    unsigned int num_rows;
    unsigned int num_attributes;
    std::unordered_map<std::string, unsigned int> attr_map;
    std::unordered_map<std::string, std::string> virtual_attr_map;
    Chunk* root;

    LeafChunk* getFirst() const;
    LeafChunk* getLast() const;
    LeafChunk* getLeaf(Key key) const;

    ValResult parseOperation(std::string attribute_1, Operation op, std::string attribute_2, std::vector<Value> &row);
    ValResult parseEquation(std::string equation, std::vector<Value> &row);
    
  public:
    Table(std::vector<Attribute> attributes);

    bool insert(Key key, std::vector<Value>& row);
    bool remove(Key key);

    ValResult getValIndex(Index index, Attribute attribute) const;
    ValResult getVal(Key key, Attribute attribute) const;
    RowResult getRowIndex(Index index) const;
    RowResult getRow(Key key) const;

    bool addAttribute(std::string name, std::string equation);
    // bool removeAttribute(std::string name);

    // void save(std::string path);
    // void load(std::string path);

    // bool apply(std::string attribute_name, std::string equation);

    void print() const;
    friend std::ostream& operator<<(std::ostream& os, const Table& table); // for debugging
};