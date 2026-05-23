
#include "table/table.hpp"

void Table::printDivider() {
  // prints simple divider
  unsigned int num_attributes = (this->virtual_attr_map.size() + this->num_attributes);
  std::cout << " ";
  for (unsigned int i = 0; i < num_attributes; i++) {
    // prints the divider
    std::cout << "|";
    for (unsigned int j = 0; j < print_width + 2; j++) {
      std::cout << "_";
    }
  }
  std::cout << "| " << std::endl;
}

void Table::printHeaders() {
  // headers in order
  std::vector<Attribute> headers(this->num_attributes);
  for (auto& pair: this->attr_map) {
    headers[pair.second] = pair.first;
  }
  for (auto& pair: this->virtual_attr_map) {
    headers.push_back(pair.first);
  }

  // prints divider for top of header
  std::cout << " ";
  for (unsigned int i = 0; i < headers.size(); i++) {
    // prints the divider
    for (unsigned int j = 0; j < print_width + 3; j++) {
      std::cout << "_";
    }
  }
  std::cout << "_ " << std::endl;


  for (Attribute attr: headers) {
    // prints all attributes
    std::cout << " | " << std::setw(print_width) << attr;
  }
  std::cout << " | " << std::endl;

  this->printDivider();
}

void Table::printValues() {
  // values in order
  for (unsigned int i = 0; i < this->num_rows; i++) {
    for (Value res: this->getRowIndex(i).row) {
      std::cout << " | " << std::setw(print_width) << std::scientific << res;
    }
    std::cout << " | " << std::endl;
  }

  this->printDivider();
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