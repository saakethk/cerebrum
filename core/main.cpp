#include <iostream>

#include "chunks/leaf_chunk.hpp"

int main() {

  // Testing leaf chunk
  LeafChunk* test = new LeafChunk(5);
  test->insert(1, {1, 2, 3, 4, 5});
  test->insert(2, {3, 2, 3, 4, 5});
  test->insert(3, {1, 2, 3, 4, 5});
  test->insert(4, {3, 2, 3, 4, 5});
  test->insert(5, {3, 2, 3, 4, 5});
  // TODO: Fix split functionality
  // bool should_split = test->insert(4, {4, 2, 3, 4, 5});
  // std::cout << should_split << std::endl;
  // std::cout << test->size().first << std::endl;
  std::cout << *test << std::endl;
  std::pair<Chunk*, Chunk*> result = test->split();
  std::cout << (*(static_cast<LeafChunk*>(result.first))) << std::endl;
  std::cout << (*(static_cast<LeafChunk*>(result.second))) << std::endl;
  return 0;

}