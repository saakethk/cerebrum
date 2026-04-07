
#include "chunk.hpp"

class LeafChunk: public Chunk {

  private:

    // vars
    unsigned int num_attributes;
    unsigned int num_filled;

    std::vector<unsigned int> keys; // chunk size
    std::vector<std::vector<double>> values; // chunk size * num attributes

    LeafChunk* next;

    // helper methods
    unsigned int searchKey(unsigned int key) const; // return local index or max val of int if not found
    unsigned int insertKey(unsigned int key); // ensures ascending order is preserved
    void insertValue(unsigned int key_index, const std::vector<double>& val);
    void insertAttributeValue(unsigned int index, double val, std::vector<double>& attribute_vals);
    bool isFull() const;

  public:
    LeafChunk(unsigned int num_attributes);

    // actions
    bool insert(unsigned int key, const std::vector<double>& val) override; // returns true if success, false otherwise
    std::pair<Chunk*, Chunk*> split() override;

    // accessors
    std::pair<unsigned int, unsigned int> size() const; // returns num_keys x num_attributes
    double get(unsigned int index, unsigned int attribute_index) const; // gets single attribute

    bool isLeaf() const override; // returns true
    const std::vector<unsigned int>& getKeys() const;
    std::vector<double> getRow(unsigned int index) const; // gets whole row of values
    LeafChunk* getNext();

    friend std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk); // for debugging
    ~LeafChunk() override;
};