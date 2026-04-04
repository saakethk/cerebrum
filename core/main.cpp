#include <iostream>

#include "chunk.hpp"

int main() {

  // Testing leaf chunk
  LeafChunk test(5);
  test.insert(1, {1, 2, 3, 4, 5});
  test.insert(2, {3, 2, 3, 4, 5});
  bool should_split = test.insert(4, {4, 2, 3, 4, 5});
  std::cout << should_split << std::endl;
  std::cout << test << std::endl;
  test.split();
  std::cout << test << std::endl;

  return 0;
}