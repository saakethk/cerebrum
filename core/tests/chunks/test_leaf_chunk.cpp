#include <iostream>
#include <limits>

#include "catch_amalgamated.hpp"
#include "chunks/leaf_chunk.hpp"

const unsigned int NUM_ATTRIBUTES = 1;
const unsigned int CHUNK_SIZE = 10;
const unsigned int NUM_INSERT = 5; // must be less than chunk size

TEST_CASE("Testing Leafchunk", "[leaf_chunk][basic]") {

  LeafChunk* leaf = new LeafChunk(CHUNK_SIZE, NUM_ATTRIBUTES);

  std::vector<Value> dummy_row;
  for (unsigned int r = 0; r < NUM_ATTRIBUTES; r++) {
    dummy_row.push_back(r);
  }

  SECTION("Testing isLeaf") {
    REQUIRE(leaf->isLeaf() == true);
  }

  SECTION("Testing Valid Insertion") {

    std::vector<Value> row_res;
    row_res.resize(NUM_ATTRIBUTES);

    for (unsigned int j = 0; j < NUM_INSERT; j++) {
      REQUIRE(leaf->insert(j, dummy_row) == Success);
      REQUIRE(leaf->exists(j) == true);
      leaf->getRow(j, row_res);
      REQUIRE(row_res == dummy_row);
    }

    REQUIRE(leaf->getNumItems() == NUM_INSERT);
  }

  SECTION("Testing Invalid Insertion") {

    for (unsigned int j = 0; j < CHUNK_SIZE; j++) {
      REQUIRE(leaf->insert(j, dummy_row) == Success);
    }

    for (unsigned int j = 0; j < CHUNK_SIZE; j++) {
      REQUIRE(leaf->insert(j, dummy_row) == Invalid);
    }

    for (unsigned int j = CHUNK_SIZE; j < (CHUNK_SIZE + CHUNK_SIZE); j++) {
      REQUIRE(leaf->insert(j, dummy_row) == Full);
    }

    REQUIRE(leaf->getNumItems() == CHUNK_SIZE);
  }

  SECTION("Testing Key Insertion Order") {

    for (unsigned int i = NUM_INSERT; i > 0 ; i--) {
      // i is key
      REQUIRE(leaf->insert(i, dummy_row) == Success);
    }
    
    for (unsigned int j = 0; j < NUM_INSERT; j++) {
      // j is offset here
      REQUIRE(leaf->getKey(j) == j + 1);
    }
  }

  SECTION("Testing Chunk Splitting") {

    for (unsigned int j = 0; j < CHUNK_SIZE; j++) {
      REQUIRE(leaf->insert(j, dummy_row) == Success);
    }

    SplitChunk res = leaf->split();
    LeafChunk* right_chunk = static_cast<LeafChunk*>(res.chunk);
    
    REQUIRE(
      (right_chunk->getNumItems() + leaf->getNumItems()) == CHUNK_SIZE);
    REQUIRE(right_chunk->getNext() == nullptr);
    REQUIRE(right_chunk->getPrevious() == leaf);
    REQUIRE(leaf->getNext() == right_chunk);
    REQUIRE(leaf->getPrevious() == nullptr);

  }

  SECTION("Testing Filling Chunk") {

    for (unsigned int j = 0; j < CHUNK_SIZE; j++) {
      CHECK(leaf->insert(j, dummy_row) == Success);
    }

    REQUIRE(leaf->isFull() == true);

  }

  SECTION("Testing Valid Deletion") {

    for (unsigned int j = 0; j < NUM_INSERT; j++) {
      REQUIRE(leaf->insert(j, dummy_row) == Success);
      REQUIRE(leaf->exists(j) == true);
      REQUIRE(leaf->remove(j) == true);
    }

    REQUIRE(leaf->getNumItems() == 0);
  }

  SECTION("Testing Invalid Deletion") {

    for (unsigned int j = 0; j < NUM_INSERT; j++) {
      REQUIRE(leaf->exists(j) == false);
      REQUIRE(leaf->remove(j) == false);
    }

    REQUIRE(leaf->getNumItems() == 0);
  }  

  delete leaf;
  
}
