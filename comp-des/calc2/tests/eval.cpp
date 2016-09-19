#include "libcalc/parse.hpp"
#include "libcalc/lex.hpp"

#include <iostream>
#include <string>
#include <cassert>
#include <fstream>

using namespace calc;

int
main(int argc, char** argv)
{
  if (argc != 2)
    return -1;
  std::string input;
  std::ifstream fs(argv[1]);
  std::cout << "Eval\n";
  while (std::getline(fs, input)){
    std::cerr << input << '\n';
    auto tokens = Lexer::build_tokens(input);
    std::string res;
    // Skip if a fatal lex error occurred.
    if (tokens.size() == 0) {
      res = "ERROR";
      input.clear();
      std::getline(fs, input);
      std::cerr << "= " << res << std::endl;
      assert(res == input);
      continue;
    }

    Primary_expr* tree = Parser::parse(tokens);
    
    // Skip if a fatal parse error occurred.
    if (!tree) {
      res = "ERROR";
      input.clear();
      std::getline(fs, input);
      std::cerr << "= " << res << std::endl;
      assert(res == input);
      continue;
    }

    Eval_visitor visitor;
    auto result = visitor.eval(tree);
    
    switch (result.type_)
    {
      case Result::Type::INT:
        res = std::to_string(result.value._int);
      break;
      case Result::Type::BOOL:
        res = result.value._bool ? "true" : "false";
      break;
      case Result::Type::ERR:
        res = "ERROR";
      break;
    }
    input.clear();
    std::getline(fs, input);
    std::cerr << "= " << res << std::endl;
    assert(res == input);

    input.clear();
  }
  fs.close();
  return 0;
}