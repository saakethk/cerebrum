#pragma once

#include <string>
#include <vector>

#include "chunk.hpp"

class Table {
  private:

    unsigned int num_attributes;
    std::vector<std::string> attributes;
    Chunk* root;

    unsigned int min_index;
    unsigned int max_index;

  public:
    Table(std::vector<std::string> attributes);

    bool insert(std::string attribute_name, unsigned int index, double val);
    bool remove(std::string attribute_name, unsigned int index, double val);

    bool addAttribute(std::string name);
    bool removeAttribute(std::string name);

    void save(std::string path);
    void load(std::string path);

    bool apply(std::string attribute_name, std::string equation);
};