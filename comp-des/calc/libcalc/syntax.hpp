#ifndef CALC_SYNTAX_HPP
#define CALC_SYNTAX_HPP

namespace calc
{


// Forward decls.
struct Visitor;
struct Eval;
struct Expr;
struct Add;
struct Sub;
struct Mul;
struct Div;
struct Mod;
struct Num;
struct Prn;


// Visitor base class.
struct Visitor
{
  virtual int visit(Add const*) = 0;
  virtual int visit(Sub const*) = 0;
  virtual int visit(Mul const*) = 0;
  virtual int visit(Div const*) = 0;
  virtual int visit(Mod const*) = 0;
  virtual int visit(Num const*) = 0;
  virtual int visit(Prn const*) = 0;
};


// Evaluation visitor.
struct Eval_visitor : Visitor
{
  int visit(Add const*);
  int visit(Sub const*);
  int visit(Mul const*);
  int visit(Div const*);
  int visit(Mod const*);
  int visit(Num const*);
  int visit(Prn const*);

  int eval(Expr const*);
};


// Prefix visitor.
struct Prefix_visitor : Visitor
{
  int visit(Add const*);
  int visit(Sub const*);
  int visit(Mul const*);
  int visit(Div const*);
  int visit(Mod const*);
  int visit(Num const*);
  int visit(Prn const*);

  int eval(Expr const*);
};


// Postfix visitor.
struct Postfix_visitor : Visitor
{
  int visit(Add const*);
  int visit(Sub const*);
  int visit(Mul const*);
  int visit(Div const*);
  int visit(Mod const*);
  int visit(Num const*);
  int visit(Prn const*);

  int eval(Expr const*);
};


// Base expression class. Follows the given grammar below.
//
// expr   ->  expr + factor |
//            expr - factor |
//            factor
// factor ->  factor * term |
//            factor / term |
//            factor * term |
//            term
// term   ->  digit | (expr)
// digit  ->  0...9
struct Expr
{
	// Ctor/Dtor
  Expr()
    : Expr(-1)
  { }

	Expr(int p)
    : Expr(nullptr, nullptr, p) 
  { }

  Expr(Expr* l, Expr* r, int p)
    : left_(l), right_(r), prec(p)
  { }

  Expr(Expr const& e)
    : left_(e.left_), right_(e.right_), prec(e.prec)
  { }

	virtual ~Expr() { }

	// Accept function for visitor.
	virtual int accept(Visitor&) const = 0;

  // Accessors.
  Expr* left() const { return left_; }
  Expr* right() const { return right_; }

  // Data members.
  Expr* left_;  // Left child.
  Expr* right_; // Right child.
  int prec;     // Precedence.
};


// The add expression.
//
// expr -> expr + factor
struct Add : Expr
{
  Add(Expr* l, Expr* r)
    : Expr::Expr(l, r, 0)
  { }

  int accept(Visitor& vis) const { return vis.visit(this); }
};


// The sub expression.
//
// expr -> expr - factor
struct Sub : Expr
{
  Sub(Expr* l, Expr* r)
    : Expr::Expr(l, r, 0)
  { }

  int accept(Visitor& vis) const { return vis.visit(this); }
};


// The mul expression.
//
// factor -> factor * term
struct Mul : Expr
{
  Mul(Expr* l, Expr* r)
    : Expr::Expr(l, r, 2)
  { }

  int accept(Visitor& vis) const { return vis.visit(this); }
};


// The div expression.
//
// factor -> factor / term
struct Div : Expr
{
  Div(Expr* l, Expr* r)
    : Expr::Expr(l, r, 1)
  { }

  int accept(Visitor& vis) const { return vis.visit(this); }
};


// The mod expression.
//
// factor -> factor % term
struct Mod : Expr
{
  Mod(Expr* l, Expr* r)
    : Expr::Expr(l, r, 1)
  { }

  int accept(Visitor& vis) const { return vis.visit(this); }
};


// The num expression.
//
// term -> digit
struct Num : Expr
{
  Num(int v)
    : Expr::Expr(4), val(v)
  { }

  int accept(Visitor& vis) const { return vis.visit(this); }

  int value() const { return val; }
  
  int val;
};


// The prn expression.
//
// term -> (expr)
struct Prn : Expr
{
  Prn(Expr* e)
    : Expr::Expr(3), expr_(e)
  { }

  int accept(Visitor& vis) const { return vis.visit(this); }

  Expr* expr() const { return expr_; }

  Expr* expr_;
};


} // end namespace calc



#endif