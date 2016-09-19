#include "parse.hpp"

#include <cctype>
#include <cstdlib>

#include <iostream>
#include <algorithm>

namespace calc
{


namespace Parser
{
  // The lookahead iterator.
  std::vector<std::string>::const_iterator look_;
}


// The operators supported.
const char operators[] = 
{
	'+',
	'-',
	'*',
	'/',
	'%',
  '(',
  ')'
};


// Number of operators supported.
const int operators_len = sizeof(operators) / sizeof(char);


// The delimiters supported.
const char delims[] =
{
	' ',
	'\t',
	'\n'
};


// Number of delimiters.
const int delims_len = sizeof(delims) / sizeof(char);


// Parses the expression from the input token string given. Returns
// the head of the parse tree.
Expr*
Parser::parse(std::vector<std::string> const& tokens)
{
	look_ = tokens.begin();
  Expr* lop = term();
  Expr* rop;
  while (look_ != tokens.end()) {
    if (*look_ == "*") {
      match("*");      
      rop = term();
      lop = mul(lop, rop);            
    }
    else if (*look_ == "/") {      
      match("/");      
      rop = term();
      lop = div(lop, rop);
    }
    else if (*look_ == "%") {      
      match("%");      
      rop = term();
      lop = mod(lop, rop);
    }
    else if (*look_ == "+") {
      match("+");      
      rop = term();
      lop = add(lop, rop);
    }
    else if (*look_ == "-") {      
      match("-");      
      rop = term();
      lop = sub(lop, rop);
    }
    else break;
  }
  return lop;
}


// Creates a new number or sub expression from the lookahead iterator. 
// If it is not pointing at a number or sub-expression it returns an
// empty expression.
Expr*
Parser::term()
{
  Expr* ret = nullptr;
	if (is_number(*look_)) {
    ret = new Num(atoi(look_->c_str()));
    ++look_;
  }
  else if (is_paren(*look_)) {
    match("(");    
    std::vector<std::string> buff;
    while (*look_ != ")")
      buff.push_back(*look_++);
    auto resume = look_;
    Expr* subexpr = parse(buff);
    ret = new Prn(subexpr);
    look_ = resume;
    match(")");
  }
  else
    std::cerr << "Error(Syntax): " << *look_ << " is not a number or sub-expression.\n";
  return ret;
}


// Creates an add expression from the left and right operands given.
Expr*
Parser::add(Expr* l, Expr* r)
{
  Expr* ret = new Add(l, r);
  return ret; 
}


// Creates a sub expression from the left and right operands given.
Expr*
Parser::sub(Expr* l, Expr* r)
{
  Expr* ret = new Sub(l, r);
  return ret; 
}


// Creates a mul expression from the left and right operands given.
// Since multiplicative operations involve precedence, a check is
// given to ensure the order of operations is correct.
Expr*
Parser::mul(Expr* l, Expr* r)
{
  Expr* ret = new Mul(nullptr, r);
  if (l->prec < ret->prec) {
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
Expr*
Parser::div(Expr* l, Expr* r)
{
  Expr* ret = new Div(nullptr, r);
  if (l->prec < ret->prec) {
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
Expr*
Parser::mod(Expr* l, Expr* r)
{
  Expr* ret = new Mod(nullptr, r);
  if (l->prec < ret->prec) {
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
  if (*look_ == s)
    ++look_;
  else
    std::cerr << "Error(Syntax): " << *look_ << " != " << s << '\n';
}


// Reports a lexing error. Gives the position of the unsupported
// character and a visual indication on the input string.
void
Lexer::report_error(char c, int pos, std::string const& input)
{
  std::cerr << "Error(Lex): Unsupported character '" << c << "' at position "
    << pos << '\n' << input << '\n';
  for (int j = 0; j < pos; j++)
    std::cerr << " ";
  std::cerr << "^\n";
}


// Scans the line of input text for supported tokens.
std::vector<std::string>
Lexer::build_tokens(std::string const& in)
{	
	std::vector<std::string> tokens;
	char c;
	std::string buf;
	for (unsigned i = 0; i < in.length(); i++)
	{	
		buf = "";
		c = in[i];
		// If digit, form a new number.
		if (is_dig(c))
		{						
			buf += c;
			while (++i < in.length() && is_dig((c =in[i])))
			{
			 	buf += c;
			}
      --i;								
			tokens.push_back(buf);
			continue;
		}

		// Check if operator.
		if (is_op(c))
		{
			buf += c;
      if (c == '-') {
  			while (++i < in.length() && is_dig((c = in[i])))
  			{
  		 		buf += c;
  			}
        --i;
      }
			tokens.push_back(buf);
      continue;
		}

		// Ignore delims/unsupported characters.
    if (is_delim(c))
      continue;

    report_error(c, i, in);
	}
	return tokens;
}


// Checks if the given char is a digit.
inline bool
is_dig(char c)
{
	return isdigit((int)c);
}


// Checks if the given char is an operator.
inline bool
is_op(char c)
{
	for (int i = 0; i < operators_len; i++)
		if (operators[i] == c)
			return true;
	return false;
}


// Checks if the given char is a delimiting character.
inline bool
is_delim(char c)
{
	for (int i = 0; i < delims_len; i++)
		if (delims[i] == c)
			return true;
	return false;
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

} // end namespace calc