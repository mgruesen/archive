#include "libcalc/parse.hpp"
#include "libcalc/lex.hpp"

#include <iostream>
#include <string>
#include <fstream>


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2


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
    if (fs)
      is = &fs;
    else 
      is = &std::cin;
  }
  if (argc == 1) {
    std::cout << "Enter 'x' to quit:\n>";
  }
  while (std::getline(*is, input)){
    if (input == "x")
      break;
    // Build token list.
    auto tokens = Lexer::build_tokens(input);
    if (!tokens.size()) {
      if (argc == 1)
        std::cerr << ">";
      continue;
    }
    
    // Build parse tree.
    Primary_expr* tree = Parser::parse(tokens);
    if (!tree) {
      if (argc == 1)
        std::cerr << ">";
      continue;
    }

    // Evaluate the parse tree and report result.
    Eval_visitor visitor;
    auto result = visitor.eval(tree);
    std::string res;
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
    std::cout << res << std::endl;
    

    // Prompt.
    input.clear();
    if (argc == 1)
      std::cerr << ">";
  }
  if (fs)
    ((std::ifstream*)is)->close();
  return 0;
}