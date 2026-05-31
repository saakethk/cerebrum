
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

template <typename T>
bool isAttribute(std::string val, 
  const std::unordered_map<std::string, T>& attr_map) {
  // checks if valid attribute

  for (auto& pair: attr_map) {
    // go through every attribute

    if (val.size() < pair.first.size()) {
      continue;
    }

    bool matches = true;
    for (unsigned int i = 0; i < pair.first.size(); i++) {
      // checks that string contains attribute
      if (pair.first[i] != val[i]) {
        matches = false;
      }
    }

    if (matches == false) {
      continue;
    }

    return true; // valid
  }

  return false; // invalid
}

// allows function above to be used for virtual attributes
template bool isAttribute<unsigned int>(
  std::string val,
  const std::unordered_map<std::string, unsigned int>& attr_map);

template bool isAttribute<std::string>(
  std::string val,
  const std::unordered_map<std::string, std::string>& attr_map);

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

bool isOperator(std::string val, 
  const std::unordered_map<std::string, OpFunc>& op_map) {
  // checks if valid supported operation

  if (op_map.find(val) == op_map.end()) {
    return false; // not valid
  }
  return true; // valid
}
