#include <iostream>
#include <limits>

#include "catch_amalgamated.hpp"
#include "chunks/leaf_chunk.hpp"

TEST_CASE("Testing Leaf Chunk Insertion", "[leaf_chunk]") {
  LeafChunk* test = new LeafChunk(5); // Should have 5 attributes

  for (unsigned int j = 0; j < Chunk::MAX_DEGREE; j++) {
    // Cannot insert more than MAX DEGREE of chunk (TODO: Make error message)
    test->insert(j, {static_cast<double>(j)});
  }
 
  auto dims = test->size();
  REQUIRE(dims.first == Chunk::MAX_DEGREE);
  REQUIRE(dims.second == 5);

  delete test;
}

TEST_CASE("Testing Leaf Chunk Order After Insertion", "[leaf_chunk]") {
  LeafChunk* test = new LeafChunk(1);

  for (unsigned int j = Chunk::MAX_DEGREE; j > 0; j--) {
    // insert in rows with index and val same
    test->insert(j, {static_cast<double>(j - 1)});
  }

  for (unsigned int i = 0; i < Chunk::MAX_DEGREE; i++) {
    // checks values are sorted
    REQUIRE(test->get(i, 0) == i);
  }

  delete test;
}

TEST_CASE("Testing Leaf Chunk Insertion Duplicate Key", "[leaf_chunk]") {
  // Inserting a duplicate key should fail
  LeafChunk* test = new LeafChunk(1); // Should have 5 attributes
  test->insert(1, {12});
  bool result = test->insert(1, {10});
  REQUIRE(result == false);
  REQUIRE(test->size().first == 1);
}

TEST_CASE("Testing Leaf Chunk Insertion Constraints", "[leaf_chunk]") {
  // Insertion of more datapoints than max chunk size should cause error
  LeafChunk* test = new LeafChunk(5);

  for (unsigned int j = 0; j < Chunk::MAX_DEGREE; j++) {
    test->insert(j, {static_cast<double>(j)}); // Fills the chunk
  }

  bool should_fail = test->insert(Chunk::MAX_DEGREE, {static_cast<double>(0)}); 
  REQUIRE(should_fail == false);
}

TEST_CASE("Testing Leaf Chunk After Split", "[leaf_chunk]") {
  LeafChunk* test = new LeafChunk(5);
  
  for (unsigned int j = 0; j < Chunk::MAX_DEGREE; j++) {
    test->insert(j, {static_cast<double>(j)});
  }

  std::pair<Chunk*, Chunk*> result = test->split();
  REQUIRE(static_cast<LeafChunk*>(result.second)->size().first == (Chunk::MAX_DEGREE - std::floor(Chunk::MAX_DEGREE / 2)));
  REQUIRE(static_cast<LeafChunk*>(result.first)->size().first == std::floor(Chunk::MAX_DEGREE / 2));

  delete result.first;
  delete result.second;
}