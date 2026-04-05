#pragma once

#include <vector>

class Chunk {
  public:
    static constexpr unsigned int MAX_DEGREE = 5;
    virtual bool isLeaf() const = 0;
    virtual bool insert(unsigned int key, const std::vector<double>& val) = 0;
    virtual std::pair<Chunk*, Chunk*> split() = 0;
    virtual ~Chunk() = default;

    // friend class Table;
};

class LeafChunk: public Chunk {

  private:

    // vars
    unsigned int num_attributes;
    unsigned int num_filled;

    std::vector<unsigned int> keys; // chunk size
    std::vector<std::vector<double>> values; // chunk size * num attributes

    LeafChunk* next;

    // helper methods
    int insertKey(unsigned int key); // ensures ascending order is preserved
    void insertValue(unsigned int key_index, const std::vector<double>& val);
    void insertAttributeValue(unsigned int index, double val, std::vector<double>& attribute_vals);
    bool isFull() const;
    std::pair<unsigned int, std::vector<double>> getValues(unsigned int index) const;

  public:
    LeafChunk(unsigned int num_attributes);

    // actions
    bool insert(unsigned int key, const std::vector<double>& val) override;
    std::pair<Chunk*, Chunk*> split() override;

    // accessors
    bool isLeaf() const override; // returns true
    std::pair<unsigned int, unsigned int> size() const; // returns num_keys x num_attributes
    const std::vector<unsigned int>& getKeys() const;

    friend std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk); // for debugging
    ~LeafChunk() override;
};
 
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