#include "type_ast.hpp"


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2

namespace calc
{

// Evaluates a literal.
Result
Literal_visitor::eval(Literal const* e)
{
  return e->accept(*this);
}


// Evaluates a boolean literal
Result
Literal_visitor::visit(Boolean_literal const* e)
{
  return Result(e->value() ? true : false);
}


// Evaluates an integer literal
Result
Literal_visitor::visit(Integer_literal const* e)
{
  return Result(e->value());
}


} // end namespace calc