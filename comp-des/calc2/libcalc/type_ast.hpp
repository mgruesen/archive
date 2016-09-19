#ifndef CALC_TYPE_AST_HPP
#define CALC_TYPE_AST_HPP

#include <climits>
#include <string>

#include "result.hpp"


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2


namespace calc
{

struct Literal;
struct Boolean_literal;
struct Integer_literal;

struct Literal_visitor
{
  Result visit(Boolean_literal const*);
  Result visit(Integer_literal const*);

  Result eval(Literal const*);
};

// The base literal class.
//
// literal          ->  boolean_literal
//                   |  integer_literal
//
// boolean_literal  ->  true 
//                   |  false
//
// integer_literal  ->  digit+
//
// digit            ->  [0-9]
struct Literal
{
  Literal() { }
  virtual ~Literal() { }

  virtual Result accept(Literal_visitor&) const = 0;
};


// The boolean literal class.
//
// boolean_literal  ->  true | false
struct Boolean_literal : Literal
{
  // Ctor.
  Boolean_literal(std::string const& v)
  { 
    validate(v);
  }

  // Accept func.
  Result accept(Literal_visitor& v) const { return v.visit(this); }

  
  // Ensures the given value is a valid keyword (true | false).
  void validate(std::string const& v)
  {
    if (v == "true")
      value_ = true;
    else if (v == "false")
      value_ = false;
    else
      throw ("Invalid boolean literal value '" + v + "'.");
  }

  // Accessor.
  long value() const { return value_ ? 1 : 0; }

  bool value_;
};


// The integer literal class.
//
// integer_literal  ->  digit+
struct Integer_literal : Literal
{
  // Ctor.
  Integer_literal(std::string const& v)
    : value_(std::stoi(v))
  { 
    validate();
  }

  // Accept func.
  Result accept(Literal_visitor& v) const { return v.visit(this); }

  // Ensures the set value is within the range of [-2^31, 2^31).
  void validate()
  {
    if (value_ < LONG_MIN || value_ >= LONG_MAX)
      throw("Integer value out of range.");
  }

  // Accessor.
  long value() const { return value_; }

  long value_;
};


} // end namespace calc

#endif