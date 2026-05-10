#pragma once

#include "chunk.hpp"

class LeafChunk: public Chunk {

  private:

    unsigned int num_attributes;
    std::vector<std::vector<Value>> values; // chunk size x num attributes
    LeafChunk* next;
    LeafChunk* previous;

    void insertValue(Index key_index, const std::vector<Value>& row);
    void insertAttributeValue(Index index, Index attr_index, Value val);

    void removeValue(Index key_index);
    void removeAttributeValue(Index index, Index attr_index);

    std::vector<Value> getRowByIndex(Index index);
    Value getRowValByIndex(Index index, Index attr_index);

  public:

    LeafChunk(unsigned int num_attributes);

    InsertStatus insert(Key key, std::vector<Value>& row);
    bool remove(Key key) override;
    SplitChunk split() override;

    bool isLeaf() const override;
    unsigned int getNumAttributes() const;
    unsigned int getNumVals() const;
    LeafChunk* getNext();
    LeafChunk* getPrevious();
    std::vector<Value> getRow(Key key);
    
    friend std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk); // for debugging
    friend class Table;
};