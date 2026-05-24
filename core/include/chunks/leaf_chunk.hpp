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

    void getRowByIndex(Index index, std::vector<Value>& row); // modifies row in-place
    Value getRowValByIndex(Index index, Index attr_index);

  public:

    LeafChunk(unsigned int chunk_size, unsigned int num_attributes);

    InsertStatus insert(Key key, const std::vector<Value>& row);
    bool remove(Key key) override;
    SplitChunk split() override;

    bool isLeaf() const override;
    void getRow(Key key, std::vector<Value>& row); // modifies row in-place
    LeafChunk* getNext();
    LeafChunk* getPrevious();
    
    friend std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk); // for debugging
    friend class Table;
};