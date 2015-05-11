#include <string>
#include <sstream>

namespace strops
{
  bool
  matches(const std::string&, const std::string&);

  bool
  contains(const std::string&, const std::string&);

  std::string
  negate_relational(const std::string&);

  bool
  is_operator(const std::string&);

  bool
  is_operator(const char);

  bool
  has_operator(const std::string&);

  bool
  is_variable(const std::string&);

  bool
  is_assignment(const std::string&);

  bool
  is_array(const std::string&);

  std::string
  get_index_expression(const std::string&);

  bool
  is_complex_array(const std::string&);

  std::string
  get_relational(const std::string&);

  std::string
  get_conditional_expression(const std::string&);  
}