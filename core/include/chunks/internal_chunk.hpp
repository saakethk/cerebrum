#pragma once

#include "chunk.hpp"

struct ChunkRes {
  bool valid;
  Chunk* chunk;
};

class InternalChunk: public Chunk {

  private:

    std::vector<Chunk*> children; // chunk size + 1

    Chunk* getNext(Key key);
    Chunk* getFirst() const;
    Chunk* getLast() const;

  public:

    InternalChunk(unsigned int chunk_size);

    InsertStatus insert(Key key);
    void insertChild(Chunk* chunk);
    InsertStatus insertChild(Key key, Chunk* chunk);

    bool remove(Key key) override;
    SplitChunk split() override;
    bool isLeaf() const override;

    friend std::ostream& operator<<(std::ostream& os, const InternalChunk& chunk); // for debugging
    friend class Table;
};
