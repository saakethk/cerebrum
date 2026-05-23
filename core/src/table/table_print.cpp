
#include "table/table.hpp"

void Table::printHeaders() {
  // headers in order
  std::vector<Attribute> headers(this->num_attributes);
  for (auto& pair: this->attr_map) {
    headers[pair.second] = pair.first;
  }

  for (Attribute attr: headers) {
    // print normal attributes
    std::cout << " | " << std::setw(print_width) << attr;
  }

  for (auto& pair: this->virtual_attr_map) {
    // print virtual attributes
    std::cout << " | " << std::setw(print_width) << pair.first;
  }
  std::cout << " | " << std::endl;

  unsigned int num_attributes = (this->virtual_attr_map.size() + headers.size());
  for (unsigned int i = 0; i < num_attributes; i++) {
    // prints the divider
    std::cout << " | ";
    for (unsigned int j = 0; j < print_width; j++) {
      std::cout << "-";
    }
  }
  std::cout << " | " << std::endl;
}

void Table::printValues() {
  // values in order
  for (unsigned int i = 0; i < this->num_rows; i++) {
    for (Value res: this->getRowIndex(i).row) {
      std::cout << " | " << std::setw(print_width) << res;
    }
    std::cout << " | " << std::endl;
  }
}

void Table::print() {
  // TODO: make uniform spacing
  this->printHeaders();
  this->printValues();
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
  os << "Table:\n";

  LeafChunk* first = table.getLast();
  while (first != nullptr) {
    // starting from first and iterates through
    os << *first;
    first = first->getPrevious();
  }

  return os;
}