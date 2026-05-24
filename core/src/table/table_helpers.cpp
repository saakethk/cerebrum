
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

  for ()

  if (attr_map.find(val) == attr_map.end()) {
    // not valid
    return false;
  }
  return true; // valid
}

ParsedAttribute parseAttribute(std::string attr_str) {
  // splits attributes with offset into respective parts

  bool has_offset = false;
  std::string attribute;
  std::string offset;

  for (char l: attr_str) {

    if (l == '_') {
      // offset start here
      has_offset = true;
      continue;
    }

    if (has_offset == true) {
      offset += l;
    } else {
      attribute += l;
    }

  }

  if (has_offset == true) {
    return {attribute, has_offset, std::stoi(offset)};
  }
  return {attribute, has_offset, 0};
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