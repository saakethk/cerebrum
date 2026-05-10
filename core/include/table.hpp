#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "chunks/chunk.hpp"
#include "chunks/leaf_chunk.hpp"

using Attribute = std::string;
using Index = unsigned int;

struct ValResult {
  bool valid;
  Value val;
};

class Table {
  private:
  
    unsigned int num_rows;
    std::unordered_map<std::string, unsigned int> attr_map;
    Chunk* root;

    LeafChunk* getFirst() const;
    LeafChunk* getLast() const;
    
  public:
    Table(std::vector<Attribute> attributes);

    bool insert(Key key, std::vector<Value>& row);
    // bool remove(Key key);
    
    ValResult getVal(Key key, Attribute attribute) const;
    std::vector<Value> getRow(Key key) const;


    // bool addAttribute(std::string name);
    // bool removeAttribute(std::string name);

    // void save(std::string path);
    // void load(std::string path);

    // bool apply(std::string attribute_name, std::string equation);

    friend std::ostream& operator<<(std::ostream& os, const Table& table); // for debugging
};