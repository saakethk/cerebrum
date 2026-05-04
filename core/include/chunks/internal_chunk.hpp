#pragma once

#include "chunk.hpp"

class InternalChunk: public Chunk {
  public:
    InternalChunk();

    // actions
    InsertStatus insert(Key key, const std::vector<Value>& val) override;
    void insertChild(Chunk* chunk);
    InsertStatus insertChild(Key key, Chunk* right_child);
    SplitChunk split() override;

    // accessors
    bool isLeaf() const override; // returns false
    std::vector<double> get(Key key); // traverses the children till leaf node reached

    friend std::ostream& operator<<(std::ostream& os, const InternalChunk& chunk); // for debugging
    ~InternalChunk() override;
};
