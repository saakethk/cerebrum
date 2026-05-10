#pragma once

#include "chunk.hpp"

struct RowResult {
  bool valid;
  std::vector<Value> value;
};

class LeafChunk: public Chunk {

  private:

    unsigned int num_attributes;
    std::vector<std::vector<Value>> values; // chunk size x num attributes
    LeafChunk* next;
    LeafChunk* previous;

    void insertValue(unsigned int key_index, const std::vector<Value>& row);
    void insertAttributeValue(unsigned int index, unsigned int attr_index, Value val);

    void removeValue(unsigned int key_index);
    void removeAttributeValue(unsigned int index, unsigned int attr_index);

    std::vector<Value> getRowByIndex(unsigned int index);
    Value getRowVal(unsigned int index, unsigned int attr_index);

  public:

    LeafChunk(unsigned int num_attributes);

    InsertStatus insert(Key key, std::vector<Value>& row);
    bool remove(Key key) override;
    SplitChunk split() override;

    bool isLeaf() const override;

    unsigned int getNumAttributes() const;
    LeafChunk* getNext();
    LeafChunk* getPrevious();
    RowResult getRow(Key key);
    
    friend std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk); // for debugging
    friend class Table;
};