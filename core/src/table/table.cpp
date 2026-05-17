
#include "table/table.hpp"

Table::Table(std::vector<Attribute> attributes) {
  // attribute size has to be known at inxitalization time
  this->num_rows = 0;
  this->num_attributes = 0;
  this->root = new LeafChunk(attributes.size());

  for (Attribute attr: attributes) {
    // initalizes mapping from attribute to index
    this->attr_map[attr] = this->num_attributes;
    this->num_attributes += 1;
  }

  // initialize ops
  this->ops["+"] = add;
  this->ops["-"] = subtract;
  this->ops["*"] = multiply;
  this->ops["/"] = divide;
  this->ops["^"] = exp;

  // for improving access time
  this->last_accessed_index = 0;
  this->last_accessed_chunk = this->getFirst();
}