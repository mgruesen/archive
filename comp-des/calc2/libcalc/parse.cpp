#include "parse.hpp"

#include <cctype>
#include <cstdlib>

#include <iostream>
#include <algorithm>


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2


namespace calc
{


namespace Parser
{
  // The lookahead iterator.
  std::vector<std::string>::const_iterator look_;
  // The column counter for error reporting.
  int col_ = 0;
}


// Keywords.
const std::string keywords[] =
{
  "true",
  "false",
};

const int num_keywords = 2;

// Parses the expression from the input token string given. Returns
// the head of the parse tree.
Primary_expr*
Parser::parse(std::vector<std::string> const& tokens)
{  
	look_ = tokens.begin();
  Primary_expr* lop;
  try
  {
    lop = primary_expr();
  }
  catch (std::string msg)
  {
    report_error(tokens.begin(), tokens.end(), msg);
    return nullptr;
  }
  catch (...)
  {
    std::cerr << "Unknown fatal error occurred.\n";
    return nullptr;
  }
  Primary_expr* rop;
  while (look_ != tokens.end()) {
    try
    {
      if (*look_ == "+") {
        rop = additive_expr();
        lop = add(lop, rop);
      }
      else if (*look_ == "-") {
        rop = additive_expr();
        lop = sub(lop, rop);
      }
      else if (*look_ == "*") {
        rop = multiplicative_expr();
        lop = mul(lop, rop);      
      }
      else if (*look_ == "/") {
        rop = multiplicative_expr();
        lop = div(lop, rop);
      }
      else if (*look_ == "%") {
        rop = multiplicative_expr();
        lop = mod(lop, rop);
      }
      else if (*look_ == "<") {
        rop = ordering_expr();
        lop = less_than(lop, rop);
      }
      else if (*look_ == ">") {
        rop = ordering_expr();
        lop = greater_than(lop, rop);
      }
      else if (*look_ == "<=") {
        rop = ordering_expr();
        lop = less_or_equal(lop, rop);
      }
      else if (*look_ == ">=") {
        rop = ordering_expr();
        lop = greater_or_equal(lop, rop);
      }
      else if (*look_ == "==") {
        rop = equality_expr();
        lop = equal(lop, rop);
      }
      else if (*look_ == "!=") {
        rop = equality_expr();
        lop = not_equal(lop, rop);
      }
      else if (*look_ == "&&") {
        rop = logical_expr();
        lop = logic_and(lop, rop);
      }
      else if (*look_ == "||") {
        rop = logical_expr();
        lop = logic_or(lop, rop);
      }
      else 
        throw std::string("Illformed expression. Expected binary operator, got '" + 
          *look_ + "'");
    }
    catch (std::string msg)
    {
      report_error(tokens.begin(), tokens.end(), msg);
      return nullptr;
    }
    catch (...)
    {
      std::cerr << "Unknown fatal error occurred.\n";
      return nullptr;
    }
  }
  col_ = 0;
  return lop;
}


// Creates a new integer literal, boolean literal, or sub expression 
// from the lookahead iterator. If it is not pointing at a number or 
// sub-expression it returns an empty expression.
Primary_expr*
Parser::primary_expr()
{
  Primary_expr* ret = nullptr;
  if (is_number(*look_)) {
    col_ += look_->length();
    ret = new Expr(new Integer_literal(*look_));
    ++look_;    
  }
  else if (is_paren(*look_)) {
    match("(");
    std::vector<std::string> buff;
    int resume_col = col_;

    while (*look_ != ")") {
      resume_col += look_->length();
      buff.push_back(*look_++);
    }
    
    auto resume_look = look_;
    Primary_expr* subexpr = parse(buff);
    ret = new Parend(subexpr);
    
    look_ = resume_look;
    col_ = resume_col;
    
    match(")");
  }
  else if (is_keyword(*look_)) {
    ret = new Expr(new Boolean_literal(*look_));
    col_ += look_->length();
    ++look_;
  }
  else if (*look_ == "!") {
    ret = unary_expr();
  }
  else
    throw std::string("Illformed expression. Expected primary_expr, got '" 
      + *look_ + "'");
  return ret;
}


// Parses a unary expr.
Primary_expr*
Parser::unary_expr()
{
  Primary_expr* ret = nullptr;
  if (*look_ == "!") {
    match("!");
    if (*look_ == "true") {
      col_ += look_->length();
      ret = new Expr(new Boolean_literal("false"));
      ++look_;
    }
    else if (*look_ == "false") {
      col_ += look_->length();
      ret = new Expr(new Boolean_literal("true"));
      ++look_;
    }
    else
      throw std::string("Illformed expression. Expected keyword, got '" 
        + *look_ + "'");
  }
  else
    throw std::string("Illformed expression. Expected unary operator, got '" 
      + *look_ + "'");
  return ret;
}


Primary_expr*
Parser::additive_expr()
{
  Primary_expr* ret = nullptr;
  if (*look_ == "+") {
    match("+");
    ret = primary_expr();
  }
  else if (*look_ == "-") {
    match("-");
    ret = primary_expr();
  }
  else
    throw std::string("Illformed expression. Expected additive_expr, got '" + *look_ + "'");
  return ret;
}


// Creates a new factor expression.
Primary_expr*
Parser::multiplicative_expr()
{
  Primary_expr* ret = nullptr;
  if (*look_ == "*") {
    match("*");
    ret = primary_expr();
  }
  else if (*look_ == "/") {
    match("/");
    ret = primary_expr();
  }
  else if (*look_ == "%") {
    match("%");
    ret = primary_expr();
  }
  else
    throw std::string("Illformed expression. Expected multiplicative_expr, got '" + *look_ + "'");
  return ret;
}


// Parses an ordering expr.
Primary_expr*
Parser::ordering_expr()
{
  Primary_expr* ret = nullptr;
  if (*look_ == ">") {
    match(">");
    ret = primary_expr();
  }
  else if (*look_ == "<") {
    match("<");
    ret = primary_expr();
  }
  else if (*look_ == ">=") {
    match(">=");
    ret = primary_expr();
  }
  else if (*look_ == "<=") {
    match("<=");
    ret = primary_expr();
  }
  else
    throw std::string("Illformed expression. Expected ordering_expr, got '" + *look_ + "'");
  return ret;  
}


// Parses an equality expr.
Primary_expr*
Parser::equality_expr()
{
  Primary_expr* ret = nullptr;
  if (*look_ == "==") {
    match("==");
    ret = primary_expr();
  }
  else if (*look_ == "!=") {
    match("!=");
    ret = primary_expr();
  }
  else
    throw std::string("Illformed expression. Expected equality_expr, got '" + *look_ + "'");
  return ret;  
}

// Parses a logical-and expr.
Primary_expr*
Parser::logical_expr()
{
  Primary_expr* ret = nullptr;
  if (*look_ == "&&") {
    match("&&");
    ret = primary_expr();
  }
  else if (*look_ == "||") {
    match("||");
    ret = primary_expr();
  }
  else
    throw std::string("Illformed expression. Expected logical_expr, got '" + *look_ + "'");
  return ret;  
}


// Creates an add expression from the left and right operands given.
Primary_expr*
Parser::add(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Add(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Creates a sub expression from the left and right operands given.
Primary_expr*
Parser::sub(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Subtract(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Creates a mul expression from the left and right operands given.
// Since multiplicative operations involve precedence, a check is
// given to ensure the order of operations is correct.
Primary_expr*
Parser::mul(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Multiply(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Creates a div expression from the left and right operands given.
// Since multiplicative operations involve precedence, a check is
// given to ensure the order of operations is correct.
Primary_expr*
Parser::div(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Divide(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Creates a mod expression from the left and right operands given.
// Since multiplicative operations involve precedence, a check is
// given to ensure the order of operations is correct.
Primary_expr*
Parser::mod(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Modulus(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Parses a logical_negation expr.
Primary_expr*
Parser::logic_neg(Primary_expr* l, Primary_expr* r)
{  
  l->right_ = new Logical_negation(r);
  return l;
}


// Parses a less_than expr.
Primary_expr*
Parser::less_than(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Less_than(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Parses a greater_than expr.
Primary_expr*
Parser::greater_than(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Greater_than(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Parses a less_or_equal expr.
Primary_expr*
Parser::less_or_equal(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Less_or_equal(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Parses a greater_or_equal expr.
Primary_expr*
Parser::greater_or_equal(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Greater_or_equal(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Parses a equal expr.
Primary_expr*
Parser::equal(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Equal(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Parses a not_equal expr.
Primary_expr*
Parser::not_equal(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Not_equal(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Parses a logical_and expr.
Primary_expr*
Parser::logic_and(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Logical_and(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Parses a logical_or expr.
Primary_expr*
Parser::logic_or(Primary_expr* l, Primary_expr* r)
{
  Primary_expr* ret = new Logical_or(nullptr, r);
  if (l->prec > ret->prec) {
    ret->left_ = l->right();
    l->right_ = ret;
    return l;
  }
  else {
    ret->left_ = l;
    return ret; 
  }
}


// Matches the lookahead iterator and the given symbol. If they are
// equal, it advances the iterator. Otherwise reports an error.
void
Parser::match(std::string const& s)
{
  if (*look_ == s) {
    col_ += look_->length();
    ++look_;    
  }
  else 
    throw std::string("Could not match '" + *look_ + "' with '" + s + "'");
}


// Reports a parsing error. Gives the position of the illegal syntax
// token and a visual indication on the input string.
void
Parser::report_error(std::vector<std::string>::const_iterator curr, 
  std::vector<std::string>::const_iterator end, std::string const& msg)
{
  
  std::string err = std::string("Error(Syntax): " + msg);
  std::string input = "";
  std::string mark = "";
  while (curr != end)
    input += std::string(*curr++ + " ");
  for (int i = 0; i <= col_; i++)
    mark += " ";
  mark += "^";
  
  std::cout << err << '\n' << input << '\n' << mark << '\n';
}


// Checks if the given string is a valid number.
inline bool
is_number(std::string const& s)
{ 
  return (std::all_of(s.begin(), s.end(), isdigit) || 
    (s[0] == '-' && std::all_of(s.begin() + 1, s.end(), isdigit)));
}


// Checks if string is a parend.
inline bool
is_paren(std::string const& s)
{
  return (s == "(" || s == ")");
}


// Checks if the string is a keyword.
inline bool
is_keyword(std::string const& s)
{
  for (int i = 0; i < num_keywords; i++)
    if (keywords[i] == s)
      return true;
  return false;
}


// Checks if the given string is a primary expr.
inline bool
is_primary(std::string const& s)
{
  return (is_number(s) || is_paren(s) || is_keyword(s));
}

} // end namespace calc