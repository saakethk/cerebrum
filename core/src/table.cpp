#include <stack>

#include "table.hpp"
#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

Table::Table(std::vector<std::string> attributes) {
  // attribute size has to be known at initalization time
  this->attributes = attributes;
  this->root = new LeafChunk(attributes.size());
}


bool Table::insert(unsigned int index, std::vector<double>& row) {
  Chunk* curr = this->root;

  std::stack<Chunk*> path;
  InternalChunk* internal = static_cast<InternalChunk*>(curr);
  while (curr->isLeaf() == false) {
    // Chunk is a InternalChunk
    curr = internal->findNextChunk(index);
  }
  
  // Chunk is a LeafChunk
  LeafChunk* leaf = static_cast<LeafChunk*>(curr);
  std::pair<bool, bool> status = leaf->insert(index, row);

  if (status.first == true) {
    // full - needs to split

    std::pair<Chunk*, Chunk*> split_leaf = leaf->split();
    if (path.empty()) {
      // leaf chunk is root node
      InternalChunk* parent = new InternalChunk(split_leaf.first);
      unsigned int split_key = static_cast<LeafChunk*>(split_leaf.second)->getKeys()[0];
      parent->insert(split_key, split_leaf.second);
      this->root = parent;
      return true; // inserted
    }

    // TODO: Case where there are already Internal Chunks

    
  } else if (status.second == true) {
    // duplicate - key already exists
    return false;

  } else {
    // no problems encountered
    // nothing needs to happen
  }
}