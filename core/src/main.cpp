// #define CATCH_CONFIG_MAIN
// #include <iostream>

// #include "catch_amalgamated.hpp"
// #include "chunks/chunk.hpp"

// TEST_CASE("Testing Leaf Chunk Insertion", "[leaf_chunk]") {
//   LeafChunk test(5); // Should have 5 attributes
//   test.insert(1, {1, 2, 3, 4, 5});
//   test.insert(2, {3, 2, 3, 4, 5});
//   test.insert(3, {7, 2, 3, 4, 5});
//   test.insert(4, {7, 2, 3, 4, 5}); // Cannot insert more than MAX DEGREE of chunk
//   std::cout << test << std::endl;
//   auto dims = test.size();
//   REQUIRE(dims.first == 4);
//   REQUIRE(dims.second == 5);
// }

// TEST_CASE("Testing Leaf Chunk Order After Insertion", "[leaf_chunk]") {
  
// }

// TEST_CASE("Testing Leaf Chunk Insertion Duplicate Key", "[leaf_chunk]") {

// }

// TEST_CASE("Testing Leaf Chunk After Split", "[leaf_chunk]") {

// }

// int main() {

//   // Testing leaf chunk
//   LeafChunk test(5);
//   test.insert(1, {1, 2, 3, 4, 5});
//   test.insert(2, {3, 2, 3, 4, 5});
//   bool should_split = test.insert(4, {4, 2, 3, 4, 5});
//   std::cout << should_split << std::endl;
//   std::cout << test << std::endl;
//   test.split();
//   std::cout << test << std::endl;

//   return 0;
// }