
#include "chunk.hpp"

class InternalChunk: public Chunk {

  private:
    std::vector<unsigned int> keys; // chunk size
    std::vector<Chunk*> children; // chunk size + 1

  public:
    InternalChunk(Chunk* left_chunk);

    // actions
    std::vector<double> get(unsigned int key); // traverses the children till leaf node reached
    bool insert(unsigned int key, std::vector<Chunk*> children);

    // accessors
    bool isLeaf() const override; // returns false

    friend std::ostream& operator<<(std::ostream& os, const InternalChunk& chunk); // for debugging
    ~InternalChunk();
};