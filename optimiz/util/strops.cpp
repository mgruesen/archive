#include <iostream>
#include <sstream>

#include "strops.hpp"

namespace strops
{

  std::string operators[] = {
    "+", 
    "-", 
    "/", 
    "*",
  };

  std::string relationals[] = {
    ">",
    "<",
    ">=",
    "<=",
    "==",
    "!="
  };

  constexpr int noperators = sizeof(operators) / sizeof(std::string);

  bool
  matches(const std::string& t, const std::string& k)
  {
    return (t.compare(k) == 0);
  }

  bool
  contains(const std::string& t, const std::string& k)
  {
    return (t.find(k) != std::string::npos);
  }

  std::string
  get_relational(const std::string& str)
  {
    std::stringstream ss(str);
    std::string token;
    while (ss >> token) {
      for (int i = 0; i < 6; i++)
      {
        if (token == relationals[i])
          return token;
      }
    }
    return "";
  }

  std::string
  negate_relational(const std::string& rel)
  {
    if (rel.length() == 1) {
      if (rel == ">") return "<=";
      else if (rel == "<") return ">=";
    }
    switch (rel[0])
    {
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

  bool 
  is_operator(const std::string& str)
  {
    for (int i = 0; i < noperators; i++) {
      if (str == operators[i]) 
        return true;
    }
    return false;
  }

  bool 
  is_operator(const char c)
  {
    for (int i = 0; i < noperators; i++) {
      if (c == operators[i][0]) 
        return true;
    }
    return false;
  }

  bool
  has_operator(const std::string& str)
  {
    std::stringstream ss(str);
    std::string buff;
    while (ss >> buff) {
      if (is_operator(buff))
        return true;
    }
    return false;
  }

  bool
  is_variable(const std::string& str)
  {
    return (!is_operator(str) && !is_array(str));
  }

  bool
  is_array(const std::string& str)
  {
    return (str.find("[") != std::string::npos);
  }

  bool
  is_complex_array(const std::string& str)
  {
    std::string temp = get_index_expression(str);
    for (unsigned i = 0; i < temp.length(); i++)
      if (is_operator(temp[i]))
        return true;
    return false;
  }

  std::string
  get_conditional_expression(const std::string& str)
  {
    return str.substr(2, str.length() - 3);
  }

  std::string
  get_index_expression(const std::string& str)
  {
    std::string ret = "";
    size_t lb, rb;
    lb = str.find("[");
    lb++;
    rb = str.find("]");
    ret = str.substr(lb, rb - lb);
    return ret;
  }
  
  bool
  is_assignment(const std::string& str)
  {
    return (str == "=");
  }
}