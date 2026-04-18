#pragma once

#include <string>
#include <vector>

#include "chunk.hpp"

class Table {
  private:
  
    std::vector<std::string> attributes;
    Chunk* root;
    
  public:
    Table(std::vector<std::string> attributes);

    bool insert(unsigned int index, std::vector<double>& row);
    // bool remove(std::string attribute_name, unsigned int index, double val);

    // bool addAttribute(std::string name);
    // bool removeAttribute(std::string name);

    // void save(std::string path);
    // void load(std::string path);

    // bool apply(std::string attribute_name, std::string equation);
};