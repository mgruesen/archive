#ifndef CALC_PARSE_HPP
#define CALC_PARSE_HPP

#include "syntax.hpp"

#include <string>
#include <vector>

namespace calc
{

namespace Parser
{

// Parses the input string. Returns the head of the 
// parse tree.
Expr* parse(std::vector<std::string> const&);

// Matches on the given symbol and advances if successful.
void match(std::string const&);

// Parses a term expr.
Expr* term();
// Parses an add expr.
Expr* add(Expr*, Expr*);
// Parses a sub expr.
Expr* sub(Expr*, Expr*);
// Parses a mul expr.
Expr* mul(Expr*, Expr*);
// Parses a div expr.
Expr* div(Expr*, Expr*);
// Parses a mod expr.
Expr* mod(Expr*, Expr*);


// The parsing lookahead iterator.
extern std::vector<std::string>::const_iterator look_;

} // end namespace Parser


namespace Lexer
{

// Scans a given input string and builds lexical tokens from it.
// Returns a vector of tokens found in the input string.
std::vector<std::string> build_tokens(std::string const&);

void report_error(char, int, std::string const&);

} // end namespace Lexer

// Checks if the string given is a valid number.
inline bool is_number(std::string const&);
// Checks if the character is a digit.
inline bool is_dig(char);
// Checks if the character is an operator.
inline bool is_op(char);
// Checks if the character is a delimiter.
inline bool is_delim(char);
// Checks if the string is a parend.
inline bool is_paren(std::string const&);

} // end namespace calc

#endif