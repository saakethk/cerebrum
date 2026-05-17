#pragma once

#include <string>
#include <unordered_set>

#include "chunks/chunk.hpp"

void add(Value& res, Value val);

void subtract(Value& res, Value val);

void divide(Value& res, Value val);

void multiply(Value& res, Value val);

void exp(Value& res, Value val);