#pragma once

#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <sstream>

#include "table/table_helpers.hpp"
#include "table/table_ops.hpp"

#include "chunks/chunk.hpp"
#include "chunks/leaf_chunk.hpp"
#include "chunks/internal_chunk.hpp"

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

    // improves indexed retrieval speed
    unsigned int last_accessed_index;
    LeafChunk* last_accessed_chunk;
    ValResult getValOffset(LeafChunk* chunk, Attribute attr, Index cur_i, Index target_i);
    RowResult getRowOffset(LeafChunk* chunk, Index cur_i, Index target_i);

    LeafChunk* getFirst() const;
    LeafChunk* getLast() const;
    LeafChunk* getLeaf(Key key) const;
    
    std::unordered_map<std::string, OpFunc> ops;
    Value evalEquation(Key key, std::string equation);

    void printHeaders();
    void printValues();
    
  public:
    Table(std::vector<Attribute> attributes);

    bool insert(Key key, std::vector<Value>& row);
    bool remove(Key key);

    ValResult getValIndex(Index index, Attribute attribute);
    // ValResult getValOffset(Key key, Attribute attribute, int offset);
    ValResult getVal(Key key, Attribute attribute, int offset);

    RowResult getRowIndex(Index index); // same as ValIndex
    // RowResult getRowOffset(Key key, int offset);
    RowResult getRow(Key key, int offset);

    bool addAttribute(std::string name, std::string equation);
    // bool removeAttribute(std::string name);

    // void save(std::string path);
    // void load(std::string path);

    // bool apply(std::string attribute_name, std::string equation);

    void print();
    friend std::ostream& operator<<(std::ostream& os, const Table& table); // for debugging
};