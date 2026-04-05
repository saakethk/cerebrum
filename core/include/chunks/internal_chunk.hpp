
#include "chunk.hpp"

class InternalChunk: public Chunk {

  private:
    std::vector<unsigned int> keys; // chunk size
    std::vector<Chunk*> children; // chunk size + 1

  public:
    InternalChunk();
    bool insert(unsigned int key, const std::vector<double>& val) override;
    bool isLeaf() const override; // returns false
    ~InternalChunk();
};