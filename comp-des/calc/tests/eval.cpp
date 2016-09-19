#include "libcalc/parse.hpp"

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
    assert(tokens.size() > 0);
    Expr* tree = Parser::parse(tokens);
    assert(tree);
    Eval_visitor evisitor;
    int result = evisitor.eval(tree);
    input.clear();
    std::getline(fs, input);
    std::cerr << "= " << result << std::endl;
    assert(std::to_string(result) == input);
    input.clear();
  }
  fs.close();
  return 0;
}