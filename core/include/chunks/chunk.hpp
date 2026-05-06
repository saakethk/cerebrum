#pragma once

#include <vector>

enum InsertStatus {
  Full,
  Invalid,
  Success
};

using Key = unsigned int;
using Value = double;

class Chunk; // forward declaration

struct SplitChunk {
  Key key;
  Chunk* left;
  Chunk* right;
};

struct KeyLoc {
  bool valid;
  unsigned int index;
};

class Chunk {

  protected:

    // vars
    unsigned int num_filled;
    std::vector<Key> keys; // chunk size
    std::vector<Chunk*> children; // chunk size + 1

  public:
    static constexpr unsigned int MAX_DEGREE = 5;

    // modifiers
    InsertStatus insertKey(unsigned int index, Key key);
    virtual InsertStatus insert(Key key, const std::vector<Value>& val) = 0;
    virtual SplitChunk split() = 0;

    // accessors
    virtual bool isLeaf() const = 0;
    bool isFull() const;

    KeyLoc searchKey(Key key) const; // return true if exists
    Chunk* getChildChunk(Key key); // traverses down tree
    Chunk* getFirstChild(); // get the first child of the node
    
    virtual ~Chunk() = default;
};