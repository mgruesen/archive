#include "libcalc/parse.hpp"

#include <iostream>
#include <string>
#include <cassert>
#include <fstream>

using namespace calc;

int
main(int argc, char** argv)
{
	std::string input;
	std::string output;
	std::ifstream fs;
	std::istream* is;
	if (argc == 1)
		is = &std::cin;
	if (argc == 2){
		fs.open(argv[1]);
		is = &fs;
	}
	std::cout << "Lex\n";
	while (std::getline(*is, input)){
		output = "";
		auto res = Lexer::build_tokens(input);
			for (auto iter = res.begin(); iter != res.end(); ++iter){
				output += *iter;
				if (iter + 1 != res.end())
					output += "|";
			}
		std::cout << input << std::endl << output << std::endl;
		input = "";
		if (argc == 2){
			std::getline(*is, input);
			assert(input == output);
		}
	}
	if (argc == 2)
		((std::ifstream*)is)->close();
	return 0;
}