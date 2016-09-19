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
  std::cout << "Postfix\n";
  while (std::getline(fs, input)){
    std::cout << input << '\n';
    auto tokens = Lexer::build_tokens(input);
    Expr* tree = Parser::parse(tokens);
    Postfix_visitor postvisitor;
    std::cout << "Result:    ";
    postvisitor.eval(tree);
    input.clear();
    std::getline(fs, input);
    std::cout << "\nShould be: " << input << '\n';
    input.clear();
  }
  fs.close();
  return 0;
}