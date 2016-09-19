# Calc
This project implements a simple interpreter that lexes and parses a given mathematical expression and generates an abstract syntax tree. After the tree has been generated it can be evaluated to give the result of the expression and print either its prefix or postfix form.

## Grammar
The grammar for this calculator languages conforms to the follow system

```
expr    ->  expr + factor | expr - factor | factor
factor  ->  factor * term | factor / term | factor % term | term
term    ->  digit | (expr)
digit   ->  0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
```


## Types
The calc language supports the following types

Type | Description
--- | ---
add | Addition
sub | Subtraction
div | Division
mod | Modulus
mul | Multiplication
prn | Parenthesis
num | Number

## Precedence
Each `expr` is assigned a value to represent its precedence which allows the proper order of operations to be carried out during evaluation.

Type | Precedence
--- | ---
add | 0
sub | 0
div | 1
mod | 1
mul | 2
prn | 3
num | 4

## Descriptions

### calc
Evaluates the mathematical value of the expression given.

### calc2sexpr
Prints the prefix form of the expression given.

### calc2post
Prints the postfix form of the expression given.

## Build
To build, run the `build.sh` script.

## Test
To use the test suite, run the `test.sh` script.

## Run
The binaries will be built out to the `build/` folder.

### calc
To run the `calc` application, move into the `build/calc` directory. The `calc` application takes 0 or 1 arguments. If given an argument, it will read from the file and evaluate each line. If no arguments are given, it will read from standard input (`cin`). While reading from standard input, entering `x` will terminate the application.

### calc2sexpr
To run the `calc2sexpr` application, move into the `build/calc2sexpr` directory. The `calc2sexpr` application takes 0 or 1 arguments. If given an argument, it will read from the file and evaluate each line. If no arguments are given, it will read from standard input (`cin`). While reading from standard input, entering `x` will terminate the application.

### calc2post
To run the `calc2post` application, move into the `build/calc2post` directory. The `calc2post` application takes 0 or 1 arguments. If given an argument, it will read from the file and evaluate each line. If no arguments are given, it will read from standard input (`cin`). While reading from standard input, entering `x` will terminate the application.
