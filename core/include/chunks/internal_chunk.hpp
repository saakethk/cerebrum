#pragma once

#include "chunk.hpp"

class InternalChunk: public Chunk {

  private:

    std::vector<Chunk*> children; // chunk size + 1

  public:

    InternalChunk();

    InternalChunk* getNext(Key key);

    InsertStatus insert(Key key);
    void insertChild(Chunk* chunk);
    void insertChild(Key key, Chunk* chunk);

    bool remove(Key key) override;
    SplitChunk split() override;

    bool isLeaf() const override;
    bool isFull() const override;

    friend std::ostream& operator<<(std::ostream& os, const InternalChunk& chunk); // for debugging
    ~InternalChunk() override;
};
