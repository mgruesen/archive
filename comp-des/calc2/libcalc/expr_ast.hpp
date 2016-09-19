#ifndef CALC_EXPR_AST_HPP
#define CALC_EXPR_AST_HPP

#include "type_ast.hpp"
#include "result.hpp"


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2


namespace calc
{


// Forward decls.
struct Visitor;
struct Primary_expr;
struct Parend;
struct Arithmetic_negation;
struct Arithmetic_identity;
struct Logical_negation;
struct Multiply;
struct Divide;
struct Modulus;
struct Add;
struct Subtract;
struct Greater_than;
struct Less_than;
struct Greater_or_equal;
struct Less_or_equal;
struct Equal;
struct Not_equal;
struct Logical_and;
struct Logical_or;
struct Expr;

struct Literal;
struct Result;  

// Visitor base class.
struct Visitor
{  
  virtual Result visit(Parend const*) = 0;
  virtual Result visit(Arithmetic_negation const*) = 0;
  virtual Result visit(Arithmetic_identity const*) = 0;
  virtual Result visit(Logical_negation const*) = 0;
  virtual Result visit(Multiply const*) = 0;
  virtual Result visit(Divide const*) = 0;
  virtual Result visit(Modulus const*) = 0;
  virtual Result visit(Add const*) = 0;
  virtual Result visit(Subtract const*) = 0;
  virtual Result visit(Greater_than const*) = 0;
  virtual Result visit(Less_than const*) = 0;
  virtual Result visit(Greater_or_equal const*) = 0;
  virtual Result visit(Less_or_equal const*) = 0;
  virtual Result visit(Equal const*) = 0;
  virtual Result visit(Not_equal const*) = 0;
  virtual Result visit(Logical_and const*) = 0;
  virtual Result visit(Logical_or const*) = 0;
  virtual Result visit(Expr const*) = 0;
};


// Integer Evaluation visitor.
struct Eval_visitor : Visitor
{
  Result visit(Parend const*);
  Result visit(Arithmetic_negation const*);
  Result visit(Arithmetic_identity const*);
  Result visit(Logical_negation const*);
  Result visit(Multiply const*);
  Result visit(Divide const*);
  Result visit(Modulus const*);
  Result visit(Add const*);
  Result visit(Subtract const*);
  Result visit(Greater_than const*);
  Result visit(Less_than const*);
  Result visit(Greater_or_equal const*);
  Result visit(Less_or_equal const*);
  Result visit(Equal const*);
  Result visit(Not_equal const*);
  Result visit(Logical_and const*);
  Result visit(Logical_or const*);
  Result visit(Expr const*);

  Result eval(Primary_expr const*);
};


// Base expression class. Follows the given grammar below.
//
// primary_expr         ->  literal
//                       |  (expr)
//
// unary_expr           ->  - unary_expr
//                       |  + unary_expr
//                       |  ! unary_expr
//                       |  primary_expr
//
// multiplicative_expr  ->  multiplicative_expr * unary_expr
//                       |  multiplicative_expr / unary_expr
//                       |  multiplicative_expr % unary_expr
//                       |  unary_expr
//
// additive_expr        ->  additive_expr + multiplicative_expr
//                       |  additive_expr - multiplicative_expr
//                       |  multiplicative_expr
//
// ordering_expr        ->  ordering_expr '>' additive_expr
//                       |  ordering_expr '<' additive_expr
//                       |  ordering_expr '>=' additive_expr
//                       |  ordering_expr '<=' additive_expr
//                       |  additive_expr
//
// equality_expr        ->  equality_expr '==' ordering_expr
//                       |  equality_expr '!=' ordering_expr
//                       |  ordering_expr
//
// logical_and_expr     ->  logical_and_expr '&&' ordering_expr
//                       |  ordering_expr
//
// logical_or_expr      ->  logical_or_expr '||' logical_and_expr
//                       |  logical_and_expr
//
// expr                 ->  logical_or_expr
//
struct Primary_expr
{
	// Ctor/Dtor
  Primary_expr()
    : Primary_expr(7)
  { }

	Primary_expr(int p)
    : Primary_expr(nullptr, nullptr, p) 
  { }

  Primary_expr(Primary_expr* l, Primary_expr* r)
    : Primary_expr(l, r, 7)
  { }

  Primary_expr(Primary_expr* op, int p)
    : Primary_expr(op, nullptr, p)
  { }

  Primary_expr(Primary_expr* l, Primary_expr* r, int p)
    : left_(l), right_(r), prec(p)
  { }

  Primary_expr(Primary_expr const& e)
    : left_(e.left_), right_(e.right_), prec(e.prec)
  { }

	virtual ~Primary_expr() { }

	// Accept function for visitor.
	virtual Result accept(Visitor&) const = 0;

  // Accessors.
  Primary_expr* operand() const { return left_; }
  Primary_expr* left() const { return left_; }
  Primary_expr* right() const { return right_; }

  // Comparators.
  bool operator==(Primary_expr* other) { return this->prec == other->prec; }
  bool operator!=(Primary_expr* other) { return !(this == other); }
  bool operator>(Primary_expr* other)  { return this->prec > other->prec; }
  bool operator<(Primary_expr* other)  { return this->prec < other->prec; }

  // Data members.
  Primary_expr* left_;   // Left child.
  Primary_expr* right_;  // Right child.
  int prec;         // Precedence.
};


// The base class for Unary expressions.
//
// unary-expr   ->  - unary-expr
//              |   + unary-expr
//              |   ! unary-expr
//              |     primary-expr
struct Unary_expr : Primary_expr
{
  using Primary_expr::Primary_expr;
};


// The parend class. Contains a sub-expression.
//
// primary-expr -> (expr)
struct Parend : Unary_expr
{
  Parend(Primary_expr* op)
    : Unary_expr::Unary_expr(op, 1)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The arithmetic negation unary expression.
//
// unary-expr -> - unary-expr
struct Arithmetic_negation : Unary_expr
{
  Arithmetic_negation(Primary_expr* op)
    : Unary_expr::Unary_expr(op, 1)
  { }

  // Accept func for visitor.
  Result accept(Visitor& v) const { return v.visit(this); }  
};


// The arithmetic identity unary expression.
//
// unary-expr -> + unary-expr
struct Arithmetic_identity : Unary_expr
{
  Arithmetic_identity(Primary_expr* op)
    : Unary_expr::Unary_expr(op, 1)
  { }

  // Accept func for visitor.  
  Result accept(Visitor& v) const { return v.visit(this); }
};


// The logical negation unary expression.
//
// unary-expr -> ! unary-expr
struct Logical_negation : Unary_expr
{
  Logical_negation(Primary_expr* op)
    : Unary_expr::Unary_expr(op, 1)
  { }

  // Accept func for visitor.
  Result accept(Visitor& v) const { return v.visit(this); }
};


// The base class for Multiplicative expressions.
//
// multiplicative-expr  ->  multiplicative-expr * unary-expr
//                      |   multiplicative-expr / unary-expr
//                      |   multiplicative-expr % unary-expr
//                      |   unary-expr
struct Multiplicative_expr : Unary_expr
{ 
  using Unary_expr::Unary_expr;
};


// The multiplication expression.
//
// multiplicative-expr  ->  multiplicative-expr * unary-expr
struct Multiply : Multiplicative_expr
{
  Multiply(Primary_expr* l, Primary_expr* r)
    : Multiplicative_expr::Multiplicative_expr(l, r, 2)
  { }

  // Accept func for visitor.
  Result accept(Visitor& v) const { return v.visit(this); }
};


// The division expression.
//
// multiplicative-expr  ->  multiplicative-expr / unary-expr
struct Divide : Multiplicative_expr
{
  Divide(Primary_expr* l, Primary_expr* r)
    : Multiplicative_expr::Multiplicative_expr(l, r, 3)
  { }


  // Accept func for visitor.
  Result accept(Visitor& v) const { return v.visit(this); }
};


// The modulus expression.
//
// multiplicative-expr  ->  multiplicative-expr % unary-expr
struct Modulus : Multiplicative_expr
{
  Modulus(Primary_expr* l, Primary_expr* r)
    : Multiplicative_expr::Multiplicative_expr(l, r, 3)
  { }

  // Accept func for visitor.
  Result accept(Visitor& v) const { return v.visit(this); }
};


// The base class for arithmetic additive expressions.
//
// additive-expr  ->  additive-expr + multplicative-expr
//                |   additive-expr - multplicative-expr
//                |   multplicative-expr
struct Additive_expr : Multiplicative_expr
{ 
  using Multiplicative_expr::Multiplicative_expr;
};


// The addition expression.
//
// additive-expr  ->  additive-expr + multplicative-expr
struct Add : Additive_expr
{
  Add(Primary_expr* l, Primary_expr* r)
    : Additive_expr::Additive_expr(l, r, 4)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The subtraction expression.
//
// additive-expr  ->  additive-expr + multplicative-expr
struct Subtract : Additive_expr
{
  Subtract(Primary_expr* l, Primary_expr* r)
    : Additive_expr::Additive_expr(l, r, 4)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The base class for Ordering expressions.
//
// ordering-expr  ->  ordering-expr '<'  additive-expr
//                |   ordering-expr '>'  additive-expr
//                |   ordering-expr '<=' additive-expr
//                |   ordering-expr '>=' additive-expr
//                |   additive-expr
struct Ordering_expr : Additive_expr
{ 
  using Additive_expr::Additive_expr;
};


// The less-than ordering expression.
//
// ordering-expr -> ordering-expr '<' additive-expr
struct Less_than : Ordering_expr
{
  Less_than(Primary_expr* l, Primary_expr* r)
    : Ordering_expr::Ordering_expr(l, r, 5)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The greater-than ordering expression.
//
// ordering-expr -> ordering-expr '>' additive-expr
struct Greater_than : Ordering_expr
{
  Greater_than(Primary_expr* l, Primary_expr* r)
    : Ordering_expr::Ordering_expr(l, r, 5)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The Less-than-or-equal ordering expression.
//
// ordering-expr -> ordering-expr '<=' additive-expr
struct Less_or_equal : Ordering_expr
{
  Less_or_equal(Primary_expr* l, Primary_expr* r)
    : Ordering_expr::Ordering_expr(l, r, 5)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The Greater-than-or-equal ordering expression.
//
// ordering-expr -> ordering-expr '>=' additive-expr
struct Greater_or_equal : Ordering_expr
{
  Greater_or_equal(Primary_expr* l, Primary_expr* r)
    : Ordering_expr::Ordering_expr(l, r, 5)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The base class for Equality expressions.
//
// equality-expr  ->  equality-expr '=='  ordering-expr
//                |   equality-expr '!='  ordering-expr
//                |   ordering-expr
struct Equality_expr : Ordering_expr
{
  using Ordering_expr::Ordering_expr;
};


// The equals expression.
//
// equality-expr -> equality-expr '==' ordering-expr
struct Equal : Equality_expr
{
  Equal(Primary_expr* l, Primary_expr* r)
    : Equality_expr::Equality_expr(l, r, 6)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The not-equal expression.
//
// equality-expr -> equality-expr '!=' ordering-expr
struct Not_equal : Equality_expr
{
  Not_equal(Primary_expr* l, Primary_expr* r)
    : Equality_expr::Equality_expr(l, r, 6)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The logical-and expression.
//
// logical-and-expr   ->  logical-and-expr  '&&'  ordering-expr
//                    |   ordering-expr  
struct Logical_and : Ordering_expr
{
  Logical_and(Primary_expr* l, Primary_expr* r)
    : Ordering_expr::Ordering_expr(l, r, 2)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The logical-or expression.
//
// logical-or-expr   ->  logical-or-expr  '&&'  logical-and-expr
//                    |  logical-and-expr  
struct Logical_or : Logical_and
{
  Logical_or(Primary_expr* l, Primary_expr* r)
    : Logical_and::Logical_and(l, r)
  { }

  Result accept(Visitor& v) const { return v.visit(this); }
};


// The bottom level expression class.
//
// expr -> logical-or-expr
struct Expr : Primary_expr
{
  Expr(Literal* v)
    : Primary_expr::Primary_expr(0), value_(v) 
  { }

  Result accept(Visitor& v) const { return v.visit(this); }

  Literal* value() const { return value_; }

  Literal* value_;
};


} // end namespace calc



#endif