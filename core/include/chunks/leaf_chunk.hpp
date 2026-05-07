#pragma once

#include "chunk.hpp"

using Value = double;

class LeafChunk: public Chunk {

  private:

    unsigned int num_attributes;
    std::vector<std::vector<Value>> values; // chunk size x num attributes
    LeafChunk* next;
    LeafChunk* previous;

    void insertValue(unsigned int key_index, const std::vector<double>& val);
    void insertAttributeValue(unsigned int index, double val, std::vector<double>& attribute_vals);

  public:

    LeafChunk(unsigned int num_attributes);

    InsertStatus insert(Key key, std::vector<Value>& row);

    bool remove(Key key) override;
    SplitChunk split() override;

    bool isLeaf() const override;
    bool isFull() const override;

    unsigned int getNumAttributes() const;
    LeafChunk* getNext();
    LeafChunk* getPrevious();

    Value getRowVal(unsigned int index, unsigned int attribute_index);
    std::vector<Value>& getRow(Key key) const;
    
    friend std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk); // for debugging
    ~LeafChunk() override;
};