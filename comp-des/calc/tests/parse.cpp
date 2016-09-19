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
	while (std::getline(fs, input)){
		std::cerr << input << '\n';
		auto res = Lexer::build_tokens(input);
		Expr* head = Parser::parse(res);
		assert(head);
		std::cerr << std::endl;
	}
	fs.close();
	return 0;
}