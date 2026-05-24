#pragma once

#include <vector>

using Key = unsigned int;
using Value = double;
using Index = unsigned int;

class Chunk; // forward declaration

struct KeyLoc {
  bool valid;
  unsigned int index;
};

struct SplitChunk {
  Key key;
  Chunk* chunk;
};

enum InsertStatus {
  Full,
  Invalid,
  Success
};

class Chunk {

  protected:

    unsigned int chunk_size;
    unsigned int num_filled;
    std::vector<Key> keys; // chunk size

    KeyLoc searchKey(Key key) const;
    bool insertKey(Index index, Key key);
    void removeKey(Index index);

  public:

    Chunk(unsigned int chunk_size);

    virtual bool remove(Key key) = 0;
    virtual SplitChunk split() = 0;

    virtual bool isLeaf() const = 0;
    bool isFull() const;

    std::vector<Key>& getKeys();
    virtual unsigned int getNumItems() const;
    
    virtual ~Chunk() = default;
    friend class Table;
};