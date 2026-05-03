#include <deque>

#include "table.hpp"
#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

Table::Table(std::vector<std::string> attributes) {
  // attribute size has to be known at initalization time
  this->attributes = attributes;
  this->root = new LeafChunk(attributes.size());
}

bool Table::insert(Key key, std::vector<Value>& row) {

  std::deque<Chunk*> path;
  path.push_back(this->root);

  while ((path.back())->isLeaf() == false) {
    // push back till leaf is reached
    path.push_back(
      (path.back())->getNextChunk(key)
    );
  }

  InsertStatus cur = (path.back())->insert(key, row);
  while (cur != Success) {
    // find node to insert into
    if (cur == Invalid) {
      return false;
    } else if (cur == Full) {
      // splits when full
      std::pair<Chunk*, Chunk*> split_chunk = (path.back())->split();

      if (path.size() == 0) {
        // if theres no parent node
        this->root = new InternalChunk();
        this->root->children.push_back()
      } else {
        // if there is a parent node

      }
      path.push_back(split_chunk.first);
      path.push_back(split_chunk.second);
    }
    // push back till leaf is reached
    path.pop_back();
  }

  // TODO check existence here
  
  // Chunk is a LeafChunk
  // LeafChunk* leaf = static_cast<LeafChunk*>(curr);
  // std::pair<bool, bool> status = leaf->insert(index, row);

  // if (status.first == true) {
  //   // full - needs to split

  //   std::pair<Chunk*, Chunk*> split_leaf = leaf->split();
  //   if (path.empty()) {
  //     // leaf chunk is root node
  //     InternalChunk* parent = new InternalChunk(split_leaf.first);
  //     unsigned int split_key = static_cast<LeafChunk*>(split_leaf.second)->getKeys()[0];
  //     parent->insert(split_key, split_leaf.second);
  //     this->root = parent;
  //     return true; // inserted
  //   }

    // TODO: Case where there are already Internal Chunks
    

    
  // } else if (status.second == true) {
  //   // duplicate - key already exists
  //   return false;

  // } else {
  //   // no problems encountered
  //   // nothing needs to happen
  // }
}