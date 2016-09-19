#include "expr_ast.hpp"

#include <iostream>


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2


namespace calc
{

///////////////////////////////////////////////////////////
//                      Eval Visitor                     //
///////////////////////////////////////////////////////////
//
// The eval visitor evaluates the value of the given expression.


// Dispatches the evaluation visitor to the expression.
Result
Eval_visitor::eval(Primary_expr const* e)
{
  try 
  {
    return e->accept(*this);
  }
  catch(std::string msg)
  {
    std::cerr << "Error(Eval): " << msg << '\n';
    return Result(msg);
  }
  return Result("Fatal evaluation error.");
}


// Evaluates a parenthetical expression.
Result
Eval_visitor::visit(Parend const* e)
{
  return eval(e->operand());
}


// Evaluates an arithmetic negation.
Result
Eval_visitor::visit(Arithmetic_negation const* e)
{
  auto res = eval(e->operand());
  res.value._int *= -1;
  return res;
}


// Evaluates an arithmetic identity.
Result
Eval_visitor::visit(Arithmetic_identity const* e)
{
  return eval(e->operand());
}


// Evaluates a logical negation.
Result
Eval_visitor::visit(Logical_negation const* e)
{
  return eval(e->operand());
}


// Evaluates a multplication operation.
Result
Eval_visitor::visit(Multiply const* e)
{
  return eval(e->left()) * eval(e->right());
}


// Evaluates a division operation.
Result
Eval_visitor::visit(Divide const* e)
{
  return eval(e->left()) / eval(e->right());
}


// Evaluates a modulus operation.
Result
Eval_visitor::visit(Modulus const* e)
{
  return eval(e->left()) % eval(e->right());
}


// Evaluates an addition operation.
Result
Eval_visitor::visit(Add const* e)
{
  return eval(e->left()) + eval(e->right());
}


// Evaluates a subtraction operation.
Result
Eval_visitor::visit(Subtract const* e)
{
  return eval(e->left()) - eval(e->right());
}


// Evaluates a greater-than comparison.
Result
Eval_visitor::visit(Greater_than const* e)
{
  return eval(e->left()) > eval(e->right());
}


// Evaluates a less-than comparison.
Result
Eval_visitor::visit(Less_than const* e)
{
  return eval(e->left()) < eval(e->right());
}


// Evaluates a greater-than-or-equal-to comparison.
Result
Eval_visitor::visit(Greater_or_equal const* e)
{
  return eval(e->left()) >= eval(e->right());
}


// Evaluates a less-than-or-equal-to comparison.
Result
Eval_visitor::visit(Less_or_equal const* e)
{
  return eval(e->left()) <= eval(e->right());
}


// Evaluates an equality comparison.
Result
Eval_visitor::visit(Equal const* e)
{
  return eval(e->left()) == eval(e->right());
}


// Evaluates an inequality comparison.
Result
Eval_visitor::visit(Not_equal const* e)
{
  return eval(e->left()) != eval(e->right());
}


// Evaluates a logical-and operation.
Result
Eval_visitor::visit(Logical_and const* e)
{
  return eval(e->left()) && eval(e->right());
}


// Evaluates a logical-or operation.
Result
Eval_visitor::visit(Logical_or const* e)
{
  return eval(e->left()) || eval(e->right());
}


// Evaluates an Expr expression.
Result
Eval_visitor::visit(Expr const* e)
{
  Literal_visitor lv;
  return e->value()->accept(lv);
}

} // end namespace calc