#include "result.hpp"

#include <iostream>
#include <string>


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2


namespace calc
{

  
// Adds two integer results. Ensures types are correct for the operation
// and throws error(s) otherwise.
Result 
operator+(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");
  
  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::BOOL:
      throw std::string("Invalid operator '+' for given type 'boolean literal'.");      
    break;
    case Result::Type::INT:
      return Result(a.value._int + b.value._int);
    break;
  }
  throw std::string("Fatal evaluation error.");
}


// Subtracts two integer results. Ensures types are correct for the operation
// and throws error(s) otherwise.
Result 
operator-(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");
    
  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::BOOL:
      throw std::string("Invalid operator '-' for given type 'boolean literal'.");      
    break;
    case Result::Type::INT:
      return Result(a.value._int - b.value._int);
    break;
  }
  throw std::string("Fatal evaluation error.");
}


// Multiplies two integer results. Ensures types are correct for the operation
// and throws error(s) otherwise.
Result 
operator*(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");
  
  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::BOOL:
      throw std::string("Invalid operator '*' for given type 'boolean literal'.");      
    break;
    case Result::Type::INT:
      return Result(a.value._int * b.value._int);
    break;
  }
  throw std::string("Fatal evaluation error.");
}


// Divides two integer results. Ensures types are correct for the operation
// and throws error(s) otherwise.
Result 
operator/(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");
  if (b.value._int == 0)
    throw std::string("Unable to divide by zero.");

  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::BOOL:
      throw std::string("Invalid operator '/' for given type 'boolean literal'.");      
    break;
    case Result::Type::INT:
      return Result(a.value._int / b.value._int);
    break;
  }
  throw std::string("Fatal evaluation error.");
}


// Computes the remainder of two integer results. Ensures types are 
// correct for the operation and throws error(s) otherwise.
Result 
operator%(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");
  if (b.value._int == 0)
    throw std::string("Unable to compute remainder from zero.");
  
  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::BOOL:
      throw std::string("Invalid operator '%' for given type 'boolean literal'.");      
    break;
    case Result::Type::INT:
      return Result(a.value._int % b.value._int);
    break;
  }  
  throw std::string("Fatal evaluation error.");
}


// Applies a greater-than comparison on two integer results. 
// Ensures types are correct for the operation and throws error(s) otherwise.
Result 
operator>(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");

  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::BOOL:
      throw std::string("Invalid operator '>=' for given type 'boolean literal'.");      
    break;
    case Result::Type::INT:
      return Result(a.value._int > b.value._int);
    break;
  }
  throw std::string("Fatal evaluation error.");
}


// Applies a greater-than-or-equal comparison on two integer results. 
// Ensures types are correct for the operation and throws error(s) otherwise.
Result 
operator>=(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");

  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::BOOL:
      throw std::string("Invalid operator '>=' for given type 'boolean literal'.");      
    break;
    case Result::Type::INT:
      return Result(a.value._int >= b.value._int);
    break;
  }  
  throw std::string("Fatal evaluation error.");
}


// Applies a less-than comparison on two integer results. 
// Ensures types are correct for the operation and throws error(s) otherwise.
Result 
operator<(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");

  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::BOOL:
      throw std::string("Invalid operator '<' for given type 'boolean literal'.");      
    break;
    case Result::Type::INT:
      return Result(a.value._int < b.value._int);
    break;
  }  
  throw std::string("Fatal evaluation error.");
}


// Applies a less-than-or-equal comparison on two integer results. 
// Ensures types are correct for the operation and throws error(s) otherwise.
Result 
operator<=(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");

  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::BOOL:
      throw std::string("Invalid operator '<=' for given type 'boolean literal'.");      
    break;
    case Result::Type::INT:
      return Result(a.value._int <= b.value._int);
    break;
  }
  throw std::string("Fatal evaluation error.");
}


// Compares two integer or boolean results for equality. Ensures types are
// correct for the operation and throws error(s) otherwise.
Result 
operator==(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");

  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::INT:
      return Result(a.value._int == b.value._int);
    break;
    case Result::Type::BOOL:
      return Result(a.value._bool == b.value._bool);
    break;
    default:
      throw std::string("Unsupported result type selected.");
  }
  
  throw std::string("Fatal evaluation error.");
}


// Compares two integer or boolean results. Ensures types are correct 
// for the operation and throws error(s) otherwise.
Result 
operator!=(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");

  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::INT:
      return Result(a.value._int != b.value._int);
    break;
    case Result::Type::BOOL:
      return Result(a.value._bool != b.value._bool);
    break;
    default:
      throw std::string("Unsupported result type selected.");
  }

  throw std::string("Fatal evaluation error.");
}


// Applies the logical-and operation to two boolean results. Ensures 
// types are correct for the operation and throws error(s) otherwise.
Result 
operator&&(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");

  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::INT:
      throw std::string("Invalid operator '&&' for given type 'integer literal'.");      
    break;
    case Result::Type::BOOL:
      return Result(a.value._bool ? b.value._bool : false);
    break;
  }

  throw std::string("Fatal evaluation error.");
}


// Applies the logical-or operation to two boolean results. Ensures 
// types are correct for the operation and throws error(s) otherwise.
Result 
operator||(Result a, Result b)
{
  // Ensure types match.
  if (a.type_ != b.type_)
    throw std::string("Invalid comparison. Operands must be of same type.");

  switch(a.type_)
  {
    case Result::Type::ERR:
      throw a.value._msg;
    break;
    case Result::Type::INT:
      throw std::string("Invalid operator '||' for given type 'integer literal'.");      
    break;
    case Result::Type::BOOL:
      if (a.value._bool == true)
        return Result(true);
      else if (b.value._bool == true)
        return Result(true);
      else
        return Result(false);
    break;
  }

  throw std::string("Fatal evaluation error.");
}


} // end namespace calc