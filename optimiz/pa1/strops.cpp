#include <iostream>

#include "strops.hpp"

namespace strops
{

  bool
  matches(const std::string& t, const std::string& k){
    return (t.compare(k) == 0);
  }

  bool
  contains(const std::string& t, const std::string& k){
    return (t.find(k) != std::string::npos);
  }

  std::string
  negate_relational(const std::string& rel){
    if(rel.length() == 1){
      if (rel == ">") return "<=";
      else if (rel == "<") return ">=";
    }
    switch(rel[0]){
      case '>': // >=
        return "<";
      break;
      case '<': // <=
        return ">";
      break;
      case '!': // !=
        return "==";
      break;
      case '=': // ==
        return "!=";
      break;
      default:
        std::cerr << "error(negate_relational): unable to determine relational '" << rel << "'\n";
      break;
    }
    return "";
  }

}