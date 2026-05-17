#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "chunks/chunk.hpp"
#include "chunks/leaf_chunk.hpp"

using Attribute = std::string;

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
    
    Value evalEquation(Key key, std::string equation);

    void printHeaders();
    void printValues();
    
  public:
    Table(std::vector<Attribute> attributes);

    bool insert(Key key, std::vector<Value>& row);
    bool remove(Key key);

    ValResult getValIndex(Index index, Attribute attribute); // TODO: store the index last accessed and go form there
    ValResult getVal(Key key, Attribute attribute);
    // ValResult getValOffset(Key key, Attribute attribute, int offset);

    RowResult getRowIndex(Index index); // same as ValIndex
    RowResult getRow(Key key);
    // RowResult getRowOffset(Key key, int offset);

    bool addAttribute(std::string name, std::string equation);
    // bool removeAttribute(std::string name);

    // void save(std::string path);
    // void load(std::string path);

    // bool apply(std::string attribute_name, std::string equation);

    void print();
    friend std::ostream& operator<<(std::ostream& os, const Table& table); // for debugging
};