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

      REQUIRE(t.insert(l, dummy) == true);
    }

    RowResult row_res;
    ValResult val_res;
    for (unsigned int x = 0; x < NUM_INSERT; x++) {
      // removing existing should succeed

      REQUIRE(t.remove(x) == true);

      row_res = t.getRow(x, 0);
      REQUIRE(row_res.valid == false);

      for (unsigned int y = 0; y < NUM_ATTRIBUTES; y++) {

        val_res = t.getVal(x, std::to_string(y), 0);
        REQUIRE(val_res.valid == false);

      }
    }

  }

  SECTION("Testing attributes") {

    for (unsigned int k = NUM_ATTRIBUTES; k < NUM_ATTRIBUTES + NUM_ATTRIBUTES; k++) {
      // removing nonexistant attributes

      REQUIRE(t.removeAttribute(std::to_string(k)) == false);
    }
    
    
    // valid attributes
    REQUIRE(t.addAttribute(std::to_string(NUM_ATTRIBUTES), "0 + 1") == true);
    REQUIRE(t.addAttribute(std::to_string(NUM_ATTRIBUTES + 1), "( 0 + 1 ) + ( 2 + 3 )") == true);
    REQUIRE(t.addAttribute(std::to_string(NUM_ATTRIBUTES + 2), "4 + ( 0 + 1 ) + ( 2 + 3 )") == true);

    // invalid attributes
    REQUIRE(t.addAttribute(std::to_string(NUM_ATTRIBUTES + 3), "1 ++ 2") == false);
    REQUIRE(t.addAttribute(std::to_string(NUM_ATTRIBUTES + 4), "1 1 + 2") == false);
    REQUIRE(t.addAttribute(std::to_string(NUM_ATTRIBUTES + 5), "+ 1 + 2") == false);

    for (unsigned int x = NUM_ATTRIBUTES; x <= NUM_ATTRIBUTES + 2; x++) {
      // removing valid added attributes

      REQUIRE(t.removeAttribute(std::to_string(x)) == true);
    }

    for (unsigned int x = NUM_ATTRIBUTES + 3; x <= NUM_ATTRIBUTES + 5; x++) {
      // removing invalid added attributes should fail

      REQUIRE(t.removeAttribute(std::to_string(x)) == false);
    }

  }
  
}