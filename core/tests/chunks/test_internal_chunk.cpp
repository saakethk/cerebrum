#include <iostream>
#include <limits>

#include "catch_amalgamated.hpp"
#include "chunks/internal_chunk.hpp"

const unsigned int CHUNK_SIZE = 10;
const unsigned int NUM_INSERT = 5; // must be less than chunk size

TEST_CASE("Testing Internal Chunk", "[internal_chunk][basic]") {

  InternalChunk* c = new InternalChunk(CHUNK_SIZE);
  Chunk* dummy = new InternalChunk(CHUNK_SIZE);

  SECTION("Testing isLeaf") {
    REQUIRE(c->isLeaf() == false);
  }

  SECTION("Testing Valid Insertion") {

    c->insertChild(dummy);

    for (unsigned int j = 0; j < NUM_INSERT; j++) {
      REQUIRE(c->insert(j, dummy) == Success);
      REQUIRE(c->exists(j) == true);
      REQUIRE(c->getNext(j) == dummy);
    }

    REQUIRE(c->getNumItems() == NUM_INSERT);

  }

  SECTION("Testing Invalid Insertion") {

    c->insertChild(dummy);

    for (unsigned int j = 0; j < CHUNK_SIZE; j++) {
      REQUIRE(c->insert(j, dummy) == Success);
    }

    for (unsigned int j = 0; j < CHUNK_SIZE; j++) {
      REQUIRE(c->insert(j, dummy) == Invalid);
    }

    for (unsigned int j = CHUNK_SIZE; j < (CHUNK_SIZE + CHUNK_SIZE); j++) {
      REQUIRE(c->insert(j, dummy) == Full);
    }

    REQUIRE(c->getNumItems() == CHUNK_SIZE);


  }

  SECTION("Testing Key Insertion Order") {

    c->insertChild(dummy);

    for (unsigned int i = NUM_INSERT; i > 0 ; i--) {
      // i is key
      REQUIRE(c->insert(i, dummy) == Success);
    }
    
    for (unsigned int j = 0; j < NUM_INSERT; j++) {
      // j is offset here
      REQUIRE(c->getKey(j) == j + 1);
    }
  }

  SECTION("Testing Chunk Splitting") {

    c->insertChild(dummy);

    for (unsigned int j = 0; j < CHUNK_SIZE; j++) {
      REQUIRE(c->insert(j, dummy) == Success);
      REQUIRE(c->getNext(j) == dummy);
    }

    REQUIRE(c->getNumItems() == CHUNK_SIZE);

    SplitChunk res = c->split();
    InternalChunk* right_chunk = static_cast<InternalChunk*>(res.chunk);
    
    // checking chunk size - 1 because the split key should be moved up
    REQUIRE((right_chunk->getNumItems() + c->getNumItems()) == (CHUNK_SIZE - 1));

  }

  SECTION("Testing Filling Chunk") {

    c->insertChild(dummy);

    for (unsigned int j = 0; j < CHUNK_SIZE; j++) {
      CHECK(c->insert(j, dummy) == Success);
    }

    REQUIRE(c->isFull() == true);

  }

  SECTION("Testing Valid Deletion") {

    c->insertChild(dummy);

    for (unsigned int j = 0; j < NUM_INSERT; j++) {
      REQUIRE(c->insert(j, dummy) == Success);
      REQUIRE(c->exists(j) == true);
      REQUIRE(c->remove(j) == true);
    }

    REQUIRE(c->getNumItems() == 0);
  }

  SECTION("Testing Invalid Deletion") {

    c->insertChild(dummy);

    for (unsigned int j = 0; j < NUM_INSERT; j++) {
      REQUIRE(c->exists(j) == false);
      REQUIRE(c->remove(j) == false);
    }

    REQUIRE(c->getNumItems() == 0);
  }  

  delete c;
  delete dummy;
  
}
