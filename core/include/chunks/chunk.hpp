#pragma once

#include <vector>

constexpr unsigned int CHUNK_SIZE = 5;

using Key = unsigned int;

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

    unsigned int num_filled;
    std::vector<Key> keys; // chunk size

    KeyLoc searchKey() const;
    bool insertKey();

  public:

    Chunk();

    virtual bool remove(Key key) = 0;
    virtual SplitChunk split() = 0;

    virtual bool isLeaf() const = 0;
    virtual bool isFull() const = 0;

    InternalChunk* getNext(Key key);
    std::vector<Key>& getKeys() const;
    unsigned int getNumItems() const;
    
    virtual ~Chunk() = default;
};