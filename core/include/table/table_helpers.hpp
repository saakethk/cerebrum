#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include <regex>
#include <queue>

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

template <typename T>
bool isAttribute(std::string val, 
  const std::unordered_map<std::string, T>& attr_map);

ParsedAttribute parseAttribute(std::string attr_str);

bool isOperator(std::string val, 
  const std::unordered_map<std::string, OpFunc>& op_map);