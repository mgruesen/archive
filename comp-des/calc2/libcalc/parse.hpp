#ifndef CALC_PARSE_HPP
#define CALC_PARSE_HPP

#include "expr_ast.hpp"
#include "type_ast.hpp"

#include <string>
#include <vector>


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2


namespace calc
{

namespace Parser
{

// Parses the input string. Returns the head of the 
// parse tree.
Primary_expr* parse(std::vector<std::string> const&);

// Matches on the given symbol and advances if successful.
void match(std::string const&);

// Parses an expr.
Primary_expr* primary_expr();
// Parses a literal.
Primary_expr* literal();
// Parses a unary expr.
Primary_expr* unary_expr();
// Parses a multiplicative expr.
Primary_expr* multiplicative_expr();
// Parses an additive expr.
Primary_expr* additive_expr();
// Parses an ordering expr.
Primary_expr* ordering_expr();
// Parses an equality expr.
Primary_expr* equality_expr();
// Parses a logical-and expr.
Primary_expr* logical_expr();

// Parses an add expr.
Primary_expr* add(Primary_expr*, Primary_expr*);
// Parses a sub expr.
Primary_expr* sub(Primary_expr*, Primary_expr*);
// Parses a mul expr.
Primary_expr* mul(Primary_expr*, Primary_expr*);
// Parses a div expr.
Primary_expr* div(Primary_expr*, Primary_expr*);
// Parses a mod expr.
Primary_expr* mod(Primary_expr*, Primary_expr*);
// Parses a logical_negation expr.
Primary_expr* logic_neg(Primary_expr*, Primary_expr*);
// Parses a less_than expr.
Primary_expr* less_than(Primary_expr*, Primary_expr*);
// Parses a greater_than expr.
Primary_expr* greater_than(Primary_expr*, Primary_expr*);
// Parses a less_or_equal expr.
Primary_expr* less_or_equal(Primary_expr*, Primary_expr*);
// Parses a greater_or_equal expr.
Primary_expr* greater_or_equal(Primary_expr*, Primary_expr*);
// Parses a equal expr.
Primary_expr* equal(Primary_expr*, Primary_expr*);
// Parses a not_equal expr.
Primary_expr* not_equal(Primary_expr*, Primary_expr*);
// Parses a logical_and expr.
Primary_expr* logic_and(Primary_expr*, Primary_expr*);
// Parses a logical_or expr.
Primary_expr* logic_or(Primary_expr*, Primary_expr*);

// Reports a parsing error.
void report_error(std::vector<std::string>::const_iterator, 
  std::vector<std::string>::const_iterator, std::string const&);

// The parsing lookahead iterator.
extern std::vector<std::string>::const_iterator look_;

} // end namespace Parser

inline bool is_number(std::string const&);
inline bool is_paren(std::string const&);
inline bool is_keyword(std::string const&);
inline bool is_primary(std::string const&);

} // end namespace calc

#endif