#pragma once

#include <string>
#include <vector>

#include "chunks/chunk.hpp"

class Table {
  private:
  
    std::vector<std::string> attributes;
    Chunk* root;
    
  public:
    Table(std::vector<std::string> attributes);

    bool insert(Key key, std::vector<Value>& row);
    // bool remove(Key key);

    // bool addAttribute(std::string name);
    // bool removeAttribute(std::string name);

    // void save(std::string path);
    // void load(std::string path);

    // bool apply(std::string attribute_name, std::string equation);

    friend std::ostream& operator<<(std::ostream& os, const Table& table); // for debugging
    ~Table();
};