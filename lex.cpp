#include <cctype>
#include <map>

using std::map;
using namespace std;

#include "lex.h"
//Keywords mapping
LexItem id_or_kw(const string& lexeme , int linenum) {
	map<string,Token> kwmap = {
		{ "writeln", WRITELN},
		{ "if", IF },
		{ "else", ELSE },
	};
	Token tt = IDENT;
	
	auto kIt = kwmap.find(lexeme);
	if (kIt != kwmap.end()) tt = kIt->second;
	else if (lexeme[0] == '$') tt = NIDENT;
	else if (lexeme[0] == '@') tt = SIDENT;
	return LexItem(tt, lexeme, linenum);
}

map<Token,string> tokenPrint = {
	{WRITELN, "WRITELN"},
	{ IF, "IF" },
	{ ELSE, "ELSE" },	
	{ IDENT, "IDENT" },
	{ NIDENT, "NIDENT" },
	{ SIDENT, "SIDENT" },
	{ ICONST, "ICONST" },
	{ RCONST, "RCONST" },
	{ SCONST, "SCONST" },
			
	{ PLUS, "PLUS" },
	{ MINUS, "MINUS" },
	{ MULT, "MULT" },
	{ DIV, "DIV" },
	{ EXPONENT, "EXPONENT" },
	{ ASSOP, "ASSOP" },
	{ NEQ, "NEQ" },
	{ NGTHAN, "NGTHAN" },
	{ NLTHAN, "NLTHAN" },
		
	{ CAT, "CAT" },
	{ SREPEAT, "SREPEAT" },
	{ SEQ, "SEQ" },
	{ SGTHAN, "SGTHAN" },
	{ SLTHAN, "SLTHAN" },
		            
	{ COMMA, "COMMA" },
	{ LPAREN, "LPAREN" },
	{ RPAREN, "RPAREN" },
	{ LBRACES, "LBRACES" },
	{ RBRACES, "RBRACES" },
		
	{ SEMICOL, "SEMICOL" },
		
	{ ERR, "ERR" },

	{ DONE, "DONE" },
};

ostream& operator<<(ostream& out, const LexItem& tok) {
	Token tt = tok.GetToken();
	out << tokenPrint[ tt ];
	if (tt == IDENT || tt == NIDENT || tt == SIDENT || tt == ICONST || tt == SCONST || tt == RCONST || tt == ERR) out << "(" << tok.GetLexeme() << ")";
	return out;
}

LexItem getNextToken(istream& in, int& linenum) {
	enum TokState { START, INID, INSTRING, ININT, INFLOAT, INCOMMENT, INSCOMPARE } lexstate = START;
	string lexeme;
	char ch, nextchar;
	Token tt;
	bool decimal = false;
	       
	while (in.get(ch)) {
		switch (lexstate) {
		case START:
			if (ch == '\n') linenum++;
                
			if (isspace(ch)) continue;

			lexeme = ch;

			if (isalpha(ch) || ch == '_') {
				lexeme = ch;
				lexstate = INID;
			} else if (ch == '$' && (isalpha(in.peek()) || in.peek() == '_')) {
				lexeme = ch;
				lexstate = INID;
			} else if (ch == '@' && (isalpha(in.peek()) || in.peek() == '_')) {
				lexeme = ch;
				lexstate = INID;
			}
			else if (ch == '\'') lexstate = INSTRING;
			else if (isdigit(ch)) lexstate = ININT;
			else if (ch == '#') {
				lexeme += ch;
				lexstate = INCOMMENT;
				in.get(ch);
			} else {
				tt = ERR;
				switch (ch) {
				case '+':
					tt = PLUS;
                    break;  
					
				case '-':
					lexeme = ch;
					nextchar = in.peek();
					nextchar = tolower(nextchar);
					if (nextchar == 'e' || nextchar == 'l' || nextchar == 'g') {
						lexstate = INSCOMPARE;
						continue;
					}
					tt = MINUS;
                    break; 
					
				case '*':
					nextchar = in.peek();
					if (nextchar == '*') {
						in.get(ch);
						tt = SREPEAT;
						break;
					}
					tt = MULT;
					break;

				case '/':
					tt = DIV;
					break;

				case '^':
					tt = EXPONENT;
					break;	
					
				case '=':
					nextchar = in.peek();
					if (nextchar == '='){
						in.get(ch);
						tt = NEQ;
						break;
					}
					tt = ASSOP;
					break;
				
				case '(':
					tt = LPAREN;
					break;	

				case ')':
					tt = RPAREN;
					break;

				case '{':
					tt = LBRACES;
					break;		

				case '}':
					tt = RBRACES;
					break;

				case ';':
					tt = SEMICOL;
					break;
					
				case ',':
					tt = COMMA;
					break;
					
				case '>':
					tt = NGTHAN;
					break;
				
				case '<':
					tt = NLTHAN;
					break;
					
				case '.':
					tt = CAT;
					break;
				}
				return LexItem(tt, lexeme, linenum);
			}
			break;	

		case INID:
			if (isalpha(ch) || isdigit(ch) || ch == '_') lexeme += ch;
			else {
				in.putback(ch);
				return id_or_kw(lexeme, linenum);
			}
			break;
					
		case INSTRING:     
			if (ch == '\n') return LexItem(ERR, lexeme, linenum);
			lexeme += ch;
			if (ch == '\'') {
				lexeme = lexeme.substr(1, lexeme.length()-2);
				return LexItem(SCONST, lexeme, linenum);
			}
			break;

		case ININT:
			if (isdigit(ch)) lexeme += ch;
			else if (ch == '.') {
				lexstate = INFLOAT;
				in.putback(ch);
			} else {
				in.putback(ch);
				return LexItem(ICONST, lexeme, linenum);
			}
			break;
		
		case INFLOAT:
			if (ch == '.' && isdigit(in.peek()) && !decimal) {
				lexeme += ch; 
				decimal = true;
			} else if (ch == '.' && !isdigit(in.peek()) && !decimal) {
				lexeme += ch;
				return LexItem(RCONST, lexeme, linenum);
			} 
			else if (isdigit(ch) && decimal) lexeme += ch;
			else if (ch == '.' && decimal){
				lexeme += ch;
				return LexItem(ERR, lexeme, linenum);
			} else {
				in.putback(ch);
				return LexItem(RCONST, lexeme, linenum);
			}
			break;
		
		case INCOMMENT:
			if(ch == '\n') {
				linenum++;
				lexstate = START;
			}
			break;
			
		case INSCOMPARE:
			ch = tolower(ch);
			lexeme += ch;
			nextchar = in.peek();
			nextchar = tolower(nextchar);
			if (ch == 'e' && nextchar == 'q') {
				in.get(ch);
				ch = tolower(ch);
				lexeme += ch;
				return LexItem(SEQ, lexeme, linenum);
			} else if (ch == 'g' && nextchar == 't') {
				in.get(ch);
				ch = tolower(ch);
				lexeme += ch;
				return LexItem(SGTHAN, lexeme, linenum);
			} else if (ch == 'l' && nextchar == 't') {
				in.get(ch);
				ch = tolower(ch);
				lexeme += ch;
				return LexItem(SLTHAN, lexeme, linenum);
			}
			else return LexItem(ERR, "Unrecognizable String Comparison Operator error", linenum);
		}
	} //end of while loop
	
	if (in.eof()) return LexItem(DONE, "", linenum);
		
	return LexItem(ERR, "some strange I/O error", linenum);
}
