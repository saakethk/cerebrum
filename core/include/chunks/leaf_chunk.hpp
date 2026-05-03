
#include "chunk.hpp"

class LeafChunk: public Chunk {

  private:

    // vars
    unsigned int num_attributes;
    std::vector<std::vector<double>> values; // chunk size * num attributes

    LeafChunk* next;

    // helper methods
    void insertValue(unsigned int key_index, const std::vector<double>& val);
    void insertAttributeValue(unsigned int index, double val, std::vector<double>& attribute_vals);

  public:
    LeafChunk(unsigned int num_attributes);

    // actions
    InsertStatus insert(Key key, const std::vector<Value>& val) override;
    SplitChunk split() override;

    // accessors
    std::pair<unsigned int, unsigned int> size() const; // returns num_keys x num_attributes
    bool isLeaf() const override; // returns true

    double get(unsigned int index, unsigned int attribute_index) const; // gets single attribute
    std::vector<double> getRow(unsigned int index) const; // gets whole row of values

    friend std::ostream& operator<<(std::ostream& os, const LeafChunk& chunk); // for debugging
    ~LeafChunk() override;
};