#include "syntax.hpp"

#include <iostream>

namespace calc
{

///////////////////////////////////////////////////////////
//                    Eval Visitor                       //
///////////////////////////////////////////////////////////
//
// The eval visitor evaluates the value of the given expression.

// Dispatches the evaluation visitor to the expression.
int
Eval_visitor::eval(Expr const* e)
{
  return e->accept(*this);
}


// Evaluates an addition operation.
int
Eval_visitor::visit(Add const* e)
{   
  return eval(e->left()) + eval(e->right());
}


// Evaluates a subtraction operation.
int
Eval_visitor::visit(Sub const* e)
{   
  return eval(e->left()) - eval(e->right()); 
}


// Evaluates a muliplication operation.
int
Eval_visitor::visit(Mul const* e)
{   
  return eval(e->left()) * eval(e->right()); 
}


// Evaluates a division operation.
int
Eval_visitor::visit(Div const* e)
{   
  return eval(e->left()) / eval(e->right()); 
}


// Evaluates a modulus operation.
int
Eval_visitor::visit(Mod const* e)
{   
  return eval(e->left()) % eval(e->right());
}


// Evaluates a number.
int
Eval_visitor::visit(Num const* e)
{   
  return e->value(); 
}


// Evaluates a sub-expression.
int
Eval_visitor::visit(Prn const* e)
{   
  return eval(e->expr());
}


///////////////////////////////////////////////////////////
//                    Prefix Visitor                     //
///////////////////////////////////////////////////////////
//
// The prefix visitor prints the entire expression in prefix
// form.

// Dispatches the prefix visitor to the expression.
int
Prefix_visitor::eval(Expr const* e)
{
  e->accept(*this);
  return 0;
}


// Prints the prefix form of the addition expression.
int
Prefix_visitor::visit(Add const* e)
{   
  std::cout << "(+ ";
  eval(e->left());
  std::cout << " ";
  eval(e->right());
  std::cout << ")";
  return 0;
}


// Prints the prefix form of the subtraction expression.
int
Prefix_visitor::visit(Sub const* e)
{   
  std::cout << "(- ";
  eval(e->left());
  std::cout << " ";
  eval(e->right());
  std::cout << ")";
  return 0; 
}


// Prints the prefix form of the multipicaiton expression.
int
Prefix_visitor::visit(Mul const* e)
{   
  std::cout << "(* ";
  eval(e->left());
  std::cout << " ";
  eval(e->right());
  std::cout << ")";
  return 0;
}


// Prints the prefix form of the division expression.
int
Prefix_visitor::visit(Div const* e)
{   
  std::cout << "(/ ";
  eval(e->left());
  std::cout << " ";
  eval(e->right());
  std::cout << ")";
  return 0;
}


// Prints the prefix form of the modulus expression.
int
Prefix_visitor::visit(Mod const* e)
{   
  std::cout << "(% ";
  eval(e->left());
  std::cout << " ";
  eval(e->right());
  std::cout << ")";
  return 0;
}


// Prints the number value.
int
Prefix_visitor::visit(Num const* e)
{   
  std::cout << e->value();
  return 0; 
}


// Prints the prefix form of the sub-expression.
int
Prefix_visitor::visit(Prn const* e)
{   
  std::cout << "(";
  eval(e->expr());
  std::cout << ")";
  return 0; 
}


///////////////////////////////////////////////////////////
//                    Postfix Visitor                    //
///////////////////////////////////////////////////////////
//
// The postfix visitor prints the entire expression in postfix
// form.

// Dispatches the postfix visitor to the expression.
int
Postfix_visitor::eval(Expr const* e)
{
  e->accept(*this);
  return 0;
}


// Prints the postfix form of the addition expression.
int
Postfix_visitor::visit(Add const* e)
{   
  std::cout << "(";
  eval(e->left());
  std::cout << " ";
  eval(e->right());
  std::cout << " +)";
  return 0;
}


// Prints the postfix form of the subtraction expression.
int
Postfix_visitor::visit(Sub const* e)
{   
  std::cout << "(";
  eval(e->left());
  std::cout << " ";
  eval(e->right());
  std::cout << " -)";
  return 0;
}


// Prints the postfix form of the multiplication expression.
int
Postfix_visitor::visit(Mul const* e)
{   
  std::cout << "(";
  eval(e->left());
  std::cout << " ";
  eval(e->right());
  std::cout << " *)";
  return 0;
}


// Prints the postfix form of the division expression.
int
Postfix_visitor::visit(Div const* e)
{   
  std::cout << "(";
  eval(e->left());
  std::cout << " ";
  eval(e->right());
  std::cout << " /)";
  return 0;
}


// Prints the postfix form of the modulus expression.
int
Postfix_visitor::visit(Mod const* e)
{   
  std::cout << "(";
  eval(e->left());
  std::cout << " ";
  eval(e->right());
  std::cout << " %)";
  return 0;
}


// Prints the number value.
int
Postfix_visitor::visit(Num const* e)
{   
  std::cout << e->value();
  return 0;
}


// Prints the postfix form of the sub-expression.
int
Postfix_visitor::visit(Prn const* e)
{   
  std::cerr << "(";
  eval(e->expr());
  std::cerr << ")";
  return 0;
}

} // end namespace calc