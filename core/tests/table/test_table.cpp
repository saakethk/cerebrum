#include <iostream>
#include <limits>

#include "catch_amalgamated.hpp"
#include "table/table.hpp"

const unsigned int NUM_ATTRIBUTES = 5;
const unsigned int NUM_INSERT = 1000; // must be less than chunk size

TEST_CASE("Testing Table", "[table][basic]") {

  // create attributes header
  std::vector<Attribute> header;
  for (unsigned int i = 0; i < NUM_ATTRIBUTES; i++) {
    header.push_back(std::to_string(i));
  }

  // create dummy row for tests
  std::vector<Value> dummy;
  for (unsigned int j = 0; j < NUM_ATTRIBUTES; j++) {
    dummy.push_back(j);
  }

  Table t = Table(header);

  SECTION("Testing insert") {
    // test insert, val accessor, row accessor

    // initial insertion should be valid
    RowResult row_res;
    ValResult val_res;
    for (unsigned int k = 0; k < NUM_INSERT; k++) {
      REQUIRE(t.insert(k, dummy) == true);

      row_res = t.getRowIndex(k);
      REQUIRE(row_res.valid == true);
      REQUIRE(row_res.row == dummy);

      row_res = t.getRow(k, 0);
      REQUIRE(row_res.valid == true);
      REQUIRE(row_res.row == dummy);

      for (unsigned int l = 0; l < NUM_ATTRIBUTES; l++) {
        
        val_res = t.getValIndex(k, std::to_string(l));
        REQUIRE(val_res.valid == true);
        REQUIRE(val_res.val == l);

        val_res = t.getVal(k, std::to_string(l), 0);
        REQUIRE(val_res.valid == true);
        REQUIRE(val_res.val == l);

      }
    }

    // duplicate insertion should fail
    for (unsigned int k = 0; k < NUM_INSERT; k++) {
      REQUIRE(t.insert(k, dummy) == false);
    }

  }

  SECTION("Testing remove") {

    for (unsigned int k = 0; k < NUM_INSERT; k++) {
      // removing nonexistant should fail

      REQUIRE(t.remove(k) == false);
    }

    for (unsigned int l = 0; l < NUM_INSERT; l++) {
      // add in dummy data

      REQUIRE(t.insert(l, dummy) == false);
    }

    for (unsigned int x = 0; x < NUM_INSERT; x++) {
      // removing existing should succeed

      REQUIRE(t.remove(x) == true);
    }

  }

  SECTION("Testing virtual attributes") {

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

bool addAttribute(std::string name, std::string equation);
bool removeAttribute(std::string name);