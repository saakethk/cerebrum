
#include "table/table.hpp"

Value Table::evalEquation(Key key, std::string equation) {

  // assumes equation is validated
  std::stringstream e(equation);
  std::queue<std::string> vals;

  std::string val;
  while (e >> val) {
    // populates queue for equation
    vals.push(val);
  }

  Value res = 0;
  std::string op = "+";
  while (vals.empty() == false) {
    // parses equation from left to right

    if (vals.front() == "(") {
      // find value of paranthesis
      vals.pop();

      unsigned int depth = 1;
      std::string sub_equation;
      while ((vals.empty() == false) && (depth > 0)) {
        std::string token = vals.front();
        vals.pop();

        if (token == "(") {
          depth++;
        } else if (token == ")") {
          depth--;
        }

        if (depth > 0) {
          if (sub_equation.empty() == false) {
            sub_equation += " ";
          }
          sub_equation += token;
        }
      }

      // evaluate the sub equation
      Value sub_res = this->evalEquation(key, sub_equation);
      this->ops[op](res, sub_res);
      
    } else if (isConstant(vals.front()) == true) {

      // if constant
      Value constant = std::stof(vals.front());
      this->ops[op](res, constant);
      vals.pop();

    } else if (isOperator(vals.front(), this->ops) == true) {
      
      // if operator
      op = vals.front();
      vals.pop();

    } else if (
      (isAttribute(vals.front(), this->attr_map) == true)
      || (isAttribute(vals.front(), this->virtual_attr_map) == true)
    ) {

      // split up offset and attribute
      ParsedAttribute p_attr = parseAttribute(vals.front());
      
      // if attribute
      Value attr_val = this->getVal(key, p_attr.attribute, p_attr.offset).val;
      this->ops[op](res, attr_val);
      vals.pop();

    } else if (vals.front() == ")") {
      vals.pop();
    }
  }
  return res;
}

bool Table::addAttribute(std::string name, std::string equation) {

  if (this->attr_map.find(name) != this->attr_map.end()) {
    // name exists in attributes
    return false;
  }

  if (this->virtual_attr_map.find(name) != this->virtual_attr_map.end()) {
    // name exists in virtual attributes
    return false;
  }

  // TODO: validate equation

  // adds the attribute to virtual_attr_map
  this->virtual_attr_map[name] = equation;

  return true;
}