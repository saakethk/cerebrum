#include <stack>
#include <iostream>

#include "table.hpp"
#include "chunks/internal_chunk.hpp"
#include "chunks/leaf_chunk.hpp"

Table::Table(std::vector<std::string> attributes) {
  // attribute size has to be known at initalization time
  this->attributes = attributes;
  this->root = new LeafChunk(attributes.size());
}

// bool Table::insert(Key key, std::vector<Value>& row) {

//   // find leaf
//   std::stack<Chunk*> path;
//   path.push(this->root);

//   while (path.top()->isLeaf() == false) {
//     // traverse down tree and find leaf
//     InternalChunk* top = static_cast<InternalChunk*>(path.top());
//     Chunk* res = top->getNext(key);
//     path.push(res);
//   }

//   // insert into leaf
//   Chunk* current = path.top();
//   LeafChunk* leaf = static_cast<LeafChunk*>(current);
//   InsertStatus status = leaf->insert(key, row);
//   path.pop();

//   if (status == Invalid) 
//     return false; // duplicate
//   if (status == Success) 
//     return true; // no split needed

//   // split internal nodes when full
//   while (status == Full) {
//     SplitChunk split = current->split();

//     if (path.empty()) {
//       // no root
//       InternalChunk* new_root = new InternalChunk();
//       new_root->insert(split.key);
//       new_root->insertChild(current);
//       new_root->insertChild(split.chunk);
//       this->root = new_root;
//       return this->insert(key, row);
//     }

//     // when parent internal node exists
//     InternalChunk* parent = static_cast<InternalChunk*>(path.top());
//     path.pop();
//     status = parent->insertChild(split.key, split.chunk);
//     current = parent;

//     if (status == Success) {
//       return this->insert(key, row);
//     }

//   }

//   if (status == Invalid) {
//     return false;
//   }
//   return true;
// }

bool Table::insert(Key key, std::vector<Value>& row) {
    std::stack<InternalChunk*> path;
    Chunk* current = this->root;
    while (!current->isLeaf()) {
        InternalChunk* parent = static_cast<InternalChunk*>(current);
        path.push(parent);
        current = parent->getNext(key);
    }

    LeafChunk* leaf = static_cast<LeafChunk*>(current);
    InsertStatus status = leaf->insert(key, row);
    if (status == Invalid) return false;
    if (status == Success) return true;

    SplitChunk pending = leaf->split();
    while (true) {
        if (path.empty()) {
            InternalChunk* new_root = new InternalChunk();
            new_root->insert(pending.key);
            new_root->insertChild(this->root);
            new_root->insertChild(pending.chunk);
            this->root = new_root;
            return this->insert(key, row);
        }

        InternalChunk* parent = path.top();
        path.pop();

        if (!parent->isFull()) {
            status = parent->insertChild(pending.key, pending.chunk);
            if (status == Invalid) return false;
            if (status == Success) {
                return this->insert(key, row);
            }
        }

        SplitChunk parent_pending = parent->split();
        if (pending.key < parent_pending.key) {
            status = parent->insertChild(pending.key, pending.chunk);
            if (status == Invalid) return false;
        } else {
            InternalChunk* right_sibling = static_cast<InternalChunk*>(parent_pending.chunk);
            status = right_sibling->insertChild(pending.key, pending.chunk);
            if (status == Invalid) return false;
        }

        pending = parent_pending;
    }
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
  os << "Table:\n";
  // visualize data
  Chunk* cur = table.root;
  while (cur->isLeaf() == false) {
    cur = static_cast<InternalChunk*>(cur)->getFirst();
  }

  LeafChunk* first = static_cast<LeafChunk*>(cur);
  while (first != nullptr) {
    os << *first;
    first = first->getNext();
  }

  return os;
}
