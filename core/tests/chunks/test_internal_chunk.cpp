#include <iostream>
#include <limits>
#include <stack>

#include "catch_amalgamated.hpp"
#include "chunks/leaf_chunk.hpp"
#include "chunks/internal_chunk.hpp"

TEST_CASE("Testing Internal Chunk Insertion and Leaf Chunk Insertion", "[leaf_chunk]") {
  Chunk* curr = new LeafChunk(5);

  std::stack<Chunk*> path;
  while (curr->isLeaf() == false) {
    // We know it is a Internal node if not leaf
    curr = static_cast<InternalChunk*>(curr)->findNextChunk(key);
  }
 
  // auto dims = test->size();
  // REQUIRE(dims.first == Chunk::MAX_DEGREE);
  // REQUIRE(dims.second == 5);

  // delete test;
}