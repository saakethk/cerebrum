#pragma once

#include <vector>

enum InsertStatus {
  Full,
  Invalid,
  Success
};

using Key = unsigned int;
using Value = double;

struct InsertKeyStatus {
  InsertStatus status;
  Key key;
};

class Chunk; // forward declaration

struct SplitChunk {
  Key key;
  Chunk* left;
  Chunk* right;
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
    InsertKeyStatus insertKey(Key key);
    virtual InsertStatus insert(Key key, const std::vector<Value>& val) = 0;
    virtual SplitChunk split() = 0;

    // accessors
    virtual bool isLeaf() const = 0;
    bool isFull() const;

    Key searchKey(Key key) const; // returns max int size if failed
    Chunk* getNextChunk(Key key);
    
    virtual ~Chunk() = default;
};