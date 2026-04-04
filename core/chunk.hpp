#pragma once

#include "table.hpp"

class Chunk {
  public:
    static constexpr unsigned int MAX_DEGREE = 3;
    virtual bool isLeaf() const = 0;
    virtual bool insert(unsigned int key, std::vector<double> val) = 0;
    virtual std::pair<Chunk*, Chunk*> split() = 0;
    virtual ~Chunk() = 0;

    friend class Table;
};

class LeafChunk: public Chunk {

  private:
    std::vector<unsigned int> keys; // chunk size
    std::vector<std::vector<double>> values; // chunk size * num attributes
    LeafChunk* next;
    unsigned int num_attributes;

    bool isFull() const;
    std::pair<unsigned int, std::vector<double>> getValues(unsigned int index) const;

  public:
    LeafChunk(unsigned int num_attributes);
    bool insert(unsigned int key, std::vector<double> val) override;
    std::pair<Chunk*, Chunk*> split() override;
    bool isLeaf() const override; // returns true

};
 
class InternalChunk: public Chunk {

  private:
    std::vector<unsigned int> keys; // chunk size
    std::vector<Chunk*> children; // chunk size + 1

  public:
    InternalChunk();
    bool insert(unsigned int key, std::vector<double> val) override;
    bool isLeaf() const override; // returns false

};