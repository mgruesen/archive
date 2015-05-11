#include <string>

namespace strops
{
  bool
  matches(const std::string& t, const std::string& k);

  bool
  contains(const std::string& t, const std::string& k);

  std::string
  negate_relational(const std::string& rel);
}