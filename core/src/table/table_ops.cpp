
#include "table/table_ops.hpp"

void add(Value& res, Value val) {
  res += val;
}

void subtract(Value& res, Value val) {
  res -= val;
}

void divide(Value& res, Value val) {
  res /= val;
}

void multiply(Value& res, Value val) {
  res *= val;
}

void exp(Value& res, Value val) {
  Value start_res = res;
  for (unsigned int i = 0; i < val; i++) {
    res *= start_res;
  }
}