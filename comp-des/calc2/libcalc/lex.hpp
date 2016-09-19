#ifndef CALC_LEX_HPP
#define CALC_LEX_HPP

#include <string>
#include <vector>


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2


namespace calc
{

namespace Lexer
{

// Scans a given input string and builds lexical tokens from it.
// Returns a vector of tokens found in the input string.
std::vector<std::string> build_tokens(std::string const&);

void report_error(int, std::string const&, std::string const&);

} // end namespace Lexer

// Checks if the string given is a valid number.
inline bool is_number(std::string const&);
// Checks if the character is a digit.
inline bool is_dig(char);
// Checks if the character is an operator.
inline bool is_op(char);
// Checks if the character is a delimiter.
inline bool is_delim(char);
// Checks if the character is an alphabetical character.
inline bool is_char(char);
// Checks if the string is a parend.
inline bool is_paren(std::string const&);
// Checks if the string is a keyword.
inline bool is_keyword(std::string const&);


} // end namespace calc

#endif