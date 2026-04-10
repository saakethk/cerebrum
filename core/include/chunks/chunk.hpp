#pragma once

#include <vector>

class Chunk {
  public:
    static constexpr unsigned int MAX_DEGREE = 5;
    
    virtual std::pair<Chunk*, Chunk*> split() = 0;
    virtual bool isLeaf() const = 0;
    virtual ~Chunk() = default;
    // friend class Table;
};