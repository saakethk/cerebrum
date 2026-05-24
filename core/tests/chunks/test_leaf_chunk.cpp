#include <iostream>
#include <limits>

#include "catch_amalgamated.hpp"
#include "chunks/leaf_chunk.hpp"

const unsigned int NUM_ATTRIBUTES = 1;
const unsigned int CHUNK_SIZE = 1;
const unsigned int NUM_INSERT = 1; // must be less than chunk size

TEST_CASE("Testing insert", "[leaf_chunk][basic]") {
    
  LeafChunk* leaf = new LeafChunk(CHUNK_SIZE, NUM_ATTRIBUTES);

  std::vector<Value> row;
  for (unsigned int r = 0; r < NUM_ATTRIBUTES; r++) {
    row.push_back(r);
  }

  // test insertion
  for (unsigned int i = 0; i < NUM_INSERT; i++) {
    leaf->insert(i, row);
  }

  std::vector<Value> row_res;
  row_res.resize(NUM_ATTRIBUTES);
  for (unsigned int j = 0; j < NUM_INSERT; j++) {
    leaf->getRow(j, row_res);
    CHECK(row_res == row);
  }

  delete leaf;
}

TEST_CASE("Testing insert", "[leaf_chunk][basic]") {

  LeafChunk* leaf = new LeafChunk(CHUNK_SIZE, NUM_ATTRIBUTES);

  std::vector<Value> row;
  for (unsigned int r = 0; r < NUM_ATTRIBUTES; r++) {
    row.push_back(r);
  }

  // test insertion
  for (unsigned int i = 0; i < NUM_INSERT; i++) {
    leaf->insert(i, row);
  }

  std::vector<Value> row_res;
  row_res.resize(NUM_ATTRIBUTES);
  for (unsigned int j = 0; j < NUM_INSERT; j++) {
    leaf->getRow(j, row_res);
    REQUIRE(row_res == row);
  }

  REQUIRE(leaf->getNumItems() == NUM_INSERT);

  delete leaf;
}

