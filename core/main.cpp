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

  table.addAttribute("test", "( stuff + stuff1 ) + ( stuff2 * stuff3 )");
  for (unsigned int j = 0; j < 1000; j++) {
    std::cout << table.getValIndex(j, "test").val << std::endl;
  }

  // row = {6, 2, 3, 4, 5};
  // for (unsigned int i = 0; i < 1000; i++) {
  //   if (table.insert(i, row) == true) {
  //     std::cout << "failed" << std::endl;
  //   }
  // }

  // for (unsigned int k = 0; k < 1000; k++) {
  //   for (Value val: table.getRow(k).row) {
  //     std::cout << val << " ";
  //   }
  //   std::cout << std::endl;
  // }

  // for (unsigned int k = 0; k < 1000; k++) {
  //   std::cout << table.getVal(k, "stuff").val << std::endl;
  // }

  // for (unsigned int k = 0; k < 1000; k++) {
  //   std::cout << table.getValIndex(k, "stuff").val << std::endl;
  // }

  // for (unsigned int k = 0; k < 1000; k++) {
  //   for (Value val: table.getRowIndex(k).row) {
  //     std::cout << val << " ";
  //   }
  //   std::cout << std::endl;
  // }

  // for (unsigned int i = 0; i < 1000; i++) {
  //   std::cout << table.remove(i) << std::endl;
  // }

  // table.print();

  // std::cout << table << std::endl;

  return 0;

}