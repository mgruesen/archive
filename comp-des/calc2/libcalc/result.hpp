#ifndef CALC_RESULT_HPP
#define CALC_RESULT_HPP

#include <string>


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2


namespace calc
{


// The result struct built out as a discriminated union. This is 
// used to pass meta-data about the current expressions being 
// operated on and also validate the operations based on type matching.
struct Result
{
  // Types of results: Integer Literal, Boolean Literal, Error.
  enum Type { INT, BOOL, ERR};

  // Possible values for results: Integer, Bool, Error.
  union Value 
  {
    // Ctors.
    Value()
      : _int(-1)
    { }
    
    Value(long v)
      : _int(v)
    { }
    
    Value(bool v)
      : _bool(v)
    { }
    
    Value(std::string v)
      : _msg(v)
    { }

    // Dtor.
    ~Value() { }
    
    long _int;        // Integer literal value.
    bool _bool;       // Boolean literal value.
    std::string _msg; // Error message.
  };

  Type type_;   // The result type.
  Value value;  // The result value.

  // Ctors.
  Result(std::string msg)
    : type_(ERR), value(msg)
  { }
  
  Result(bool v)
    : type_(BOOL), value(v)
  { }
  
  Result(long v)
    : type_(INT), value(v)
  { }

  Result(Result const& o)
  {
    switch(o.type_) {
      case INT:
        this->type_ = INT;
        this->value._int = o.value._int;
      break;
      case BOOL:
        this->type_ = BOOL;
        this->value._bool = o.value._bool;
      break;
      case ERR:
      this->type_ = ERR;
      this->value._msg = o.value._msg;
      break;
    }
  }
  
  // Dtor.
  ~Result() { }
};

// Overloaded operators.
Result operator+(Result, Result);
Result operator-(Result, Result);
Result operator*(Result, Result);
Result operator/(Result, Result);
Result operator%(Result, Result);
Result operator>(Result, Result);
Result operator>=(Result, Result);
Result operator<(Result, Result);
Result operator<=(Result, Result);
Result operator==(Result, Result);
Result operator!=(Result, Result);
Result operator&&(Result, Result);
Result operator||(Result, Result);

} // end namespace calc

#endif