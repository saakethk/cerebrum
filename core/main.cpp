#include <iostream>

#include "chunks/leaf_chunk.hpp"
#include "table.hpp"

int main() {

  // Testing leaf chunk
  // LeafChunk* test = new LeafChunk(5);
  // test->insert(1, {1, 2, 3, 4, 5});
  // test->insert(2, {3, 2, 3, 4, 5});
  // test->insert(3, {1, 2, 3, 4, 5});
  // test->insert(4, {3, 2, 3, 4, 5});
  // test->insert(5, {3, 2, 3, 4, 5});
  // TODO: Fix split functionality
  // bool should_split = test->insert(4, {4, 2, 3, 4, 5});
  // std::cout << should_split << std::endl;
  // std::cout << test->size().first << std::endl;
  // std::cout << *test << std::endl;
  // SplitChunk result = test->split();
  // std::cout << (*(static_cast<LeafChunk*>(result.left))) << std::endl;
  // std::cout << (*(static_cast<LeafChunk*>(result.right))) << std::endl;

  Table table = Table({"stuff", "stuff1", "stuff2", "stuff3", "stuff4"});

  std::vector<Value> row = {1, 2, 3, 4, 5};
  for (unsigned int i = 0; i < 1000; i++) {
    table.insert(i, row);
  }

  row = {6, 2, 3, 4, 5};
  for (unsigned int i = 0; i < 1000; i++) {
    table.insert(i, row);
  }

  std::cout << table << std::endl;

  return 0;

}