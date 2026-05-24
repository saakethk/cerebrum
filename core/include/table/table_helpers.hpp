#pragma once

#include <string>
#include <unordered_set>
#include <regex>

#include "chunks/chunk.hpp"

enum Operation {
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  EXPONENT
};

struct ParsedAttribute {
  std::string attribute;
  bool has_offset;
  int offset;
};

using OpFunc = void(*)(Value& res, Value val);

bool isConstant(std::string val);

bool isAttribute(std::string val, 
  std::unordered_map<std::string, unsigned int>& attr_map);

ParsedAttribute parseAttribute(std::string attr_str);

bool isVirtualAttribute(std::string val, 
  std::unordered_map<std::string, std::string>& virtual_attr_map);

bool isOperator(std::string val, 
  std::unordered_map<std::string, OpFunc>& op_map);