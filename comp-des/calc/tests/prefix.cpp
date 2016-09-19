#include "libcalc/parse.hpp"

#include <iostream>
#include <string>
#include <fstream>

using namespace calc;

int
main(int argc, char** argv)
{
  if (argc != 2)
    return -1;
  std::string input;
  std::ifstream fs(argv[1]);
  while (std::getline(fs, input)){
    std::cout << input << '\n';
    auto tokens = Lexer::build_tokens(input);
    Expr* tree = Parser::parse(tokens);
    Prefix_visitor pvisitor;
    std::cout << "Result:    ";
    pvisitor.eval(tree);
    input.clear();
    std::getline(fs, input);
    std::cout << "\nShould be: " << input << '\n';
    input.clear();
  }
  fs.close();
  return 0;
}