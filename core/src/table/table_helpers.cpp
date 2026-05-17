
#include "table/table_helpers.hpp"

bool isConstant(std::string val) {
  // checks if string represent a float
  std::regex is_float(R"([+-]?([0-9]*\.[0-9]+|[0-9]+))");

  if (std::regex_match(val, is_float) == true) {
    // is a constant
    return true;
  }

  // is not a constant
  return false;
}

bool isAttribute(std::string val, 
  std::unordered_map<std::string, unsigned int>& attr_map) {
  // checks if in attribute map

  if (attr_map.find(val) == attr_map.end()) {
    // not valid
    return false;
  }
  return true; // valid
}

bool isVirtualAttribute(std::string val, 
  std::unordered_map<std::string, std::string>& virtual_attr_map) {
  // checks if in virtual attribute map

  if (virtual_attr_map.find(val) == virtual_attr_map.end()) {
    // not valid
    return false;
  }
  return true; // valid
}

bool isOperator(std::string val, 
  std::unordered_map<std::string, OpFunc>& op_map) {
  // checks if valid supported operation

  if (op_map.find(val) == op_map.end()) {
    // not valid
    return false;
  }
  return true; // valid
}