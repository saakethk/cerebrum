#include <iostream>

#include "catch_amalgamated.hpp"
#include "chunks/leaf_chunk.hpp"

TEST_CASE("Testing Leaf Chunk Insertion", "[leaf_chunk]") {
  LeafChunk test(5); // Should have 5 attributes
  test.insert(1, {1, 2, 3, 4, 5});
  test.insert(2, {3, 2, 3, 4, 5});
  test.insert(3, {7, 2, 3, 4, 5});
  test.insert(4, {7, 2, 3, 4, 5}); // Cannot insert more than MAX DEGREE of chunk
  auto dims = test.size();
  REQUIRE(dims.first == 4);
  REQUIRE(dims.second == 5);
}

TEST_CASE("Testing Leaf Chunk Order After Insertion", "[leaf_chunk]") {
  LeafChunk* test = new LeafChunk(1);
  test->insert(5, {5});
  test->insert(4, {4});
  test->insert(3, {3});
  test->insert(2, {2});
  test->insert(1, {1});
  test->getValue(0, 0);
  REQUIRE(test->getValue(0, 0) == 1);
  REQUIRE(test->getValue(1, 0) == 2);
  REQUIRE(test->getValue(2, 0) == 3);
  REQUIRE(test->getValue(3, 0) == 4);
  REQUIRE(test->getValue(4, 0) == 5);
}

// TEST_CASE("Testing Leaf Chunk Insertion Duplicate Key", "[leaf_chunk]") {

// }

TEST_CASE("Testing Leaf Chunk After Split", "[leaf_chunk]") {
  LeafChunk* test = new LeafChunk(5);
  test->insert(1, {1, 2, 3, 4, 5});
  test->insert(2, {3, 2, 3, 4, 5});
  test->insert(3, {1, 2, 3, 4, 5});
  test->insert(4, {3, 2, 3, 4, 5});
  test->insert(5, {1, 2, 3, 4, 5});
  std::pair<Chunk*, Chunk*> result = test->split();
  REQUIRE(static_cast<LeafChunk*>(result.first)->size().first == 2);
  REQUIRE(static_cast<LeafChunk*>(result.second)->size().first == 3);
}