# Calc2
This project implements a simple interpreter that lexes and parses a given arithmetic, relational, and/or logical expression and generates an abstract syntax tree. After the tree has been generated it can be evaluated to give the result of the expression.

## Grammar
The grammar for this calculator languages conforms to the following system

```
primary_expr         ->  literal
                      |  (expr)

unary_expr           ->  - unary_expr
                      |  + unary_expr
                      |  ! unary_expr
                      |  primary_expr

multiplicative_expr  ->  multiplicative_expr * unary_expr
                      |  multiplicative_expr / unary_expr
                      |  multiplicative_expr % unary_expr
                      |  unary_expr

additive_expr        ->  additive_expr + multiplicative_expr
                      |  additive_expr - multiplicative_expr
                      |  multiplicative_expr

ordering_expr        ->  ordering_expr '>' additive_expr
                      |  ordering_expr '<' additive_expr
                      |  ordering_expr '>=' additive_expr
                      |  ordering_expr '<=' additive_expr
                      |  additive_expr

equality_expr        ->  equality_expr '==' ordering_expr
                      |  equality_expr '!=' ordering_expr
                      |  ordering_expr

logical_and_expr     ->  logical_and_expr '&&' ordering_expr
                      |  ordering_expr

logical_or_expr      ->  logical_or_expr '||' logical_and_expr
                      |  logical_and_expr

expr                 ->  logical_or_expr

```


## Types
The calc language supports the following literal types

Type | Description
--- | ---
bool | Boolean literal with value of `true` or `false`
int | Integer literal with a value between `[-2^32, 2^32)`

## Precedence
Each `expr` is assigned a value to represent its precedence which allows the proper order of operations to be carried out during evaluation.

Type | Precedence
--- | ---
Literal | 0
Unary | 1
Multiplicative | 2/3
Additive | 4
Ordering | 5
Equality | 6

## Descriptions

### calc
Evaluates the mathematical value of the expression given.

## Build
To build, run the `build.sh` script.

## Test
To use the test suite, run the `test.sh` script.

## Run
The binaries will be built out to the `build/` folder.

### calc
To run the `calc` application, move into the `build/calc` directory. The `calc` application takes 0 or 1 arguments. If given an argument, it will read from the file and evaluate each line. If no arguments are given, it will read from standard input (`cin`). While reading from standard input, entering `x` will terminate the application.