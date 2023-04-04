# New Perl-Like Language
*Simple Perl-Like Language by [Truong Dang](https://youtu.be/WhOarQhY8FI)*

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

## Hot it works

## How it is built:

## How to run:
- Clone this repo to your local IDE (VSCode).
- 

## Technology: 
