#include <iostream>

#include "catch_amalgamated.hpp"
#include "chunks/chunk.hpp"

int main() {

  // Testing leaf chunk
  LeafChunk test(5);
  test.insert(1, {1, 2, 3, 4, 5});
  test.insert(2, {3, 2, 3, 4, 5});
  test.insert(1, {1, 2, 3, 4, 5});
  test.insert(2, {3, 2, 3, 4, 5});
  // TODO: Fix split functionality
  bool should_split = test.insert(4, {4, 2, 3, 4, 5});
  std::cout << should_split << std::endl;
  std::cout << test.size().first << std::endl;
  std::cout << test << std::endl;
  test.split();
  std::cout << test << std::endl;

  return 0;

}