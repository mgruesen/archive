#include "libcalc/parse.hpp"

#include <iostream>
#include <string>
#include <fstream>

using namespace calc;

int
main(int argc, char** argv)
{
  std::string input;
  std::ifstream fs;
  std::istream* is;
  if (argc == 1)
    is = &std::cin;
  if (argc == 2){
    fs.open(argv[1]);
    is = &fs;
  }
  if (argc == 1) {
    std::cout << "Enter 'x' to quit:\n>";
  }
  while (std::getline(*is, input)){
    if (input == "x")
      break;
    auto tokens = Lexer::build_tokens(input);
    Expr* tree = Parser::parse(tokens);
    Eval_visitor evisitor;
    int result = evisitor.eval(tree);
    std::cout << result << '\n';
    input.clear();
    if (argc == 1)
      std::cerr << ">";
  }
  if (argc == 2)
    ((std::ifstream*)is)->close();
  return 0;
}