#pragma once

#include "chunk.hpp"

struct ChunkRes {
  bool valid;
  Chunk* chunk;
};

class InternalChunk: public Chunk {

  private:

    std::vector<Chunk*> children; // chunk size + 1

  public:

    InternalChunk();

    ChunkRes getNext(Key key);

    InsertStatus insert(Key key);
    void insertChild(Chunk* chunk);
    void insertChild(Key key, Chunk* chunk);

    bool remove(Key key) override;
    SplitChunk split() override;

    bool isLeaf() const override;

    friend std::ostream& operator<<(std::ostream& os, const InternalChunk& chunk); // for debugging
};
