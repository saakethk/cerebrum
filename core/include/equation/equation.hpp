#pragma once

#include "table.hpp"

#include <string>
#include <unordered_map>

class Equation {

  private:
    std::string equation_str;

  public:
    Equation(std::string equation_str);
    Value evaluate(std::unordered_map<std::string, unsigned int>& attr_map,
      std::unordered_map<std::string, std::string>& virtual_attr_map,
      std::vector<Value>& row);

};