# New Perl-Like Language
*New Simple Perl-Like Language by [Truong Dang](https://youtu.be/WhOarQhY8FI)*

## Contributors:
- [Truong Dang](https://www.linkedin.com/in/2dt/)
- [Dr. Bassel Arafeh](https://scholar.google.com/citations?user=JYGNtvIAAAAJ&hl=en)
- [TA. Hessamaldin Mohammadi](https://www.linkedin.com/in/hessam-mohammadi-a3633976/)

## Language Definition:

### Lexical Analyzer

- The syntax definitions of this Perl-like language are given below using [Extended Backus–Naur form (EBNF)](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form) notations:

| Usage      | Notation |
| ----------- | :-----------: |
| a character *x* |	x |
| an escaped character | \x |
| M followed by N | MN |
| **zero or more** occurrences of M | M* |
| **one or more** occurrences of M | M+ |
| **zero or one** occurrence of M | M? |
| definition | = |
| termination	| ; |
| alternation |	\| |
| optional |	[ ... ] |
| repetition |	{ ... } |
| grouping |	( ... ) |
| terminal string |	' ... ' |
| exception |	- |


- Identifiers (**IDENT**)
  - **IDENT** := [ Letter _ ] {( Letter | Digit | _ )}
  - Letter := [ a-z A-Z ]
  - Digit := [0-9]
  
- The variables are either numeric or string scalar variables. Numeric variables start by "$" while a string variables start by "@". Both are followed by an IDENT.
  - **NIDENT** := $ IDENT
  - **SIDENT** := @ IDENT
  
- Integer constants (**ICONST**)
  - **ICONST** := [0-9]+
  
- Real constants (**RCONST**)
  - **RCONST** := ([0-9]+)\.([0-9]*)
  
- String constants (**SCONST**)
  - **SCONST** := \'[anything]+\'
  
- Reserved words: *writeln*, *if*, and *else*
  - Tokens, respectively: **WRITELN**, **IF**, **ELSE**
  
- Terminals: semicolon, comma, left parenthesis, right parenthesis, left braces, and right braces
  - Tokens, respectively: **SEMICOL**, **COMMA**, **LPAREN**, **RPAREN**, **LBRACES**, and **RBRACES**
  
- Comment is defined by anything following “#” to the end of line and does not have a token

- Operators: +, -, *, /, ^, =, ==, >, <, . (dot), ** (repeat), -eq (string equality), -lt (string less than), and -gt (string greater than)
  - Tokens, respectively: **PLUS**, **MINUS**, **MULT**, **DIV**, **EXPONENT**, **ASSOP**, **NEQ**, **NGTHAN**, **NLTHAN**, **CAT**, **SREPEAT**, **SEQ**, **SLTHAN**, and **SGTHAN**
  
- Error (**ERR**)

- End of file (**DONE**)

### Recursive-Descent Parser

- Definition (using recursion):

```
1. Prog ::= StmtList
2. StmtList ::= Stmt ;{ Stmt; }
3. Stmt ::= AssignStme | WriteLnStmt | IfStmt
4. WriteLnStmt ::= writeln (ExprList)
5. IfStmt ::= if (Expr) ‘{‘ StmtList ‘}’ [ else ‘{‘ StmtList ‘}’ ]
6. AssignStmt ::= Var = Expr
7. Var ::= NIDENT | SIDENT
8. ExprList ::= Expr { , Expr }
9. Expr ::= RelExpr [(-eq|==) RelExpr ]
10. RelExpr ::= AddExpr [ ( -lt | -gt | < | > ) AddExpr ]
11. AddExpr :: MultExpr { ( + | - | .) MultExpr }
12. MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
13. ExponExpr ::= UnaryExpr { ^ UnaryExpr }
14. UnaryExpr ::= [( - | + )] PrimaryExpr
15. PrimaryExpr ::= IDENT | SIDENT | NIDENT | ICONST | RCONST | SCONST | (Expr)
```

- Table of Operators Precedence Levels:

| Precedence | Operator | Description | Associativity |
| :-----------: | :-----------: | :-----------: | :-----------: |
| 1 |	Unary + , - | Unary plus and minus | Right to Left |
| 2 |	^ | Exponent | Right to Left |
| 3 |	\*, /, \*\* | Multiplication, Division, and String repetition | Left to Right |
| 4 |	+, -, . (Dot) | Addition, Subtraction, and String concatenation | Left to Right |
| 5 |	<, >, -gt, -lt | - Numeric Relational<br> - String Relational | no cascading |
| 6 |	==, -eq | - Numeric Equality<br> - String Equality | no cascading |

### Interpreter

- Implement `Value` class member functions and overloaded operators from `val.h`

- The **interpreter** provides:
  - Perform syntax analysis of the input source code statement by statement, then execute the stamte if there is no error.
  - Build information of variables types in the symbol table for all defined variables.
  - Evaluate expressions and determine values and types.

## How it's built:

- Coding language: C++
- Libraries/Frameworks: `string`, `queue`, `map`, `iomanip`, `stdexcept`, `cmath`, `sstream`, and `iostream`

## How to run:

- Clone this repo to your local IDE (VSCode).
- Run `program.cpp` by command line (including the testcase file name).
