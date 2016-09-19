#include "lex.hpp"

#include <iostream>
#include <sstream>


// Michael Gruesen
// Compiler Design Fall 2015
// Calc-2


namespace calc
{

// The operators supported.
const char operators[] = 
{
  '+',
  '-',
  '*',
  '/',
  '%',
  '(',
  ')',
  '!',
  '&',
  '|',
  '<',
  '>',
  '='
};


// Keywords.
const std::string keywords[] =
{
  "true",
  "false",
};

const int keywords_len = 2;

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


// Reports a lexing error. Gives the position of the unsupported
// character and a visual indication on the input string.
void
Lexer::report_error(int pos, std::string const& input, std::string const& msg)
{
  std::string err = std::string("Error(Lex): " + msg + "\n" + input);
  int i = 0;
  std::string mark = "";
  while (i++ < pos)
    mark += " ";
  while (isalnum(input[i++]))
    mark += "^";
  mark += "^";
  std::cout << err << '\n' << mark << '\n';
}


// Scans the line of input text for supported tokens.
std::vector<std::string>
Lexer::build_tokens(std::string const& in)
{ 
  std::stringstream stream(in);
  std::vector<std::string> tokens;
  char c;
  int i = 0;
  std::string buf;
  while (stream.get(c))
  {    
    // Ignore delims.
    if (is_delim(c)) {
      ++i;
      continue;
    }
 
    buf = "";
    // If digit, form a new integer.
    if (is_dig(c))
    {           
      do
      {
        buf += c;
        ++i;
      } while (stream.get(c) && is_dig(c));
      
      stream.unget();

      tokens.push_back(buf);
      continue;
    }

    // Check if operator.
    if (is_op(c))
    {
      // Attempt to form a negative number. When this fails the token
      // for subtraction is simply added.
      if (c == '-') 
      {        
        do
        {
          buf += c;
          ++i;
        } while (stream.get(c) && is_dig(c));
        
        stream.unget();
      }
      // Attempt to form a less-than or less-than-or-equal token.
      else if (c == '<') 
      {
        buf += c;
        ++i;
        if (stream.get(c) && c == '=')
        {
          buf += c;        
          ++i;
        }
        else
          stream.unget();
      }
      // Attempt to form a greater-than or greater-than-or-equal token.
      else if (c == '>')
      {
        buf += c;
        ++i;
        if (stream.get(c) && c == '=')
        {
          buf += c;
          ++i;  
        }
        else
          stream.unget();          
      }
      // Attempt to form an equal-to token. Will fix itself if not
      // properly formed.
      else if (c == '=')
      {
        buf += c;
        ++i;
        if (stream.get(c) && c == '=')
        {
          buf += c;
          ++i;
        }
        else 
        {
          stream.unget();
          report_error(i-1, in, std::string("Illformed '==' token found at position "
            + std::to_string(i-1) + std::string(". Adding '='.")));
          buf += '=';
        }
      }
      // Attempt to form a logical-negation or not-equal-to token.
      else if (c == '!')
      {
        buf += c;
        ++i;
        if (stream.get(c) && c == '=')
        {
          buf += c;
          ++i;
        }
        else
          stream.unget();          
      }
      // Attempt to form a logical-and token. Will fix itself if not
      // properly formed.
      else if (c == '&')
      {
        buf += c;
        ++i;
        if (stream.get(c) && c == '&')
        {
          buf += c;
          ++i;
        }
        else 
        {
          stream.unget();
          report_error(i-1, in, std::string("Illformed '&&' token found at position "
            + std::to_string(i-1) + std::string(". Adding '&'.")));
          buf += '&';
        }
      }
      // Attempt to form a logical-or token. Will fix itself if not
      // properly formed.
      else if (c == '|')
      {
        buf += c;
        ++i;
        if (stream.get(c) && c == '|')
        {
          buf += c;          
          ++i;
        }
        else 
        {
          stream.unget();
          report_error(i-1, in, std::string("Illformed '||' token found at position "
            + std::to_string(i-1) + std::string(". Adding '|'.")));
          buf += '|';
        }
      }
      // Add the operator
      else {
        buf += c;
        ++i;
      }

      tokens.push_back(buf);
      continue;
    }

    // Attempt to form a keyword.
    if (is_char(c))
    {
      int start = i;
      // Consume until no more characters.
      do 
      {
        buf += c;
        ++i;
      } while (stream.get(c) && is_char(c));
      
      // Replace last char from stream.
      stream.unget();

      // If valid, add.
      if (is_keyword(buf))
        tokens.push_back(buf);
      // Else, report error.
      else 
      {        
        report_error(start, in, std::string("Unsupported keyword '" + buf 
          + "' found."));
      }

      continue;
    }

    // Report unsupported character.
    report_error(i, in, std::string("Unsupported character '" + std::to_string(c) + "' found at position "
      + std::to_string(i)));
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


// Checks if given char is an alphabetical character.
inline bool
is_char(char c)
{
  return isalpha(c);
}


// Checks if the given token is a keyword.
inline bool
is_keyword(std::string const& token)
{
  for (int i = 0; i < keywords_len; i++)
    if (keywords[i] == token)
      return true;
  return false;
} 


} // end namespace calc