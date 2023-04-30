#include "parserInt.h"

map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> ValueResults;
queue <Value> * ValQue;

bool disable = false, error = false;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if (pushed_back) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if (pushed_back) abort();
		pushed_back = true;
		pushed_token = t;	
	}
}

static int error_count = 0;
int ErrCount() {return error_count;}

void ParseError(int line, string msg) {
	++error_count;
	cout << error_count << ". Line # " << line << ": " << msg << endl;
}

//Program is: Prog ::= StmtList
bool Prog(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == DONE && tok.GetLinenum() <= 1) {
		ParseError(line, "Empty File");
		return true;
	}
	Parser::PushBackToken(tok);

	bool f1 = StmtList(in, line); 		
	if (!f1 || error) {
		ParseError(line, "Missing Program");
		return false;
	}
	else return true;
}//End of Prog

//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line) {
	LexItem tok;
	bool status = Stmt(in, line);
	while (status) {
		tok = Parser::GetNextToken(in, line);
		if (tok == DONE) {
            cout << endl << "(" << "DONE" << ")" << endl;
			Parser::PushBackToken(tok);
			return true;
		} else if (tok == RBRACES) {
			Parser::PushBackToken(tok);
			return true;
		}
		
		if (tok != SEMICOL) {
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		status = Stmt(in, line);
	}
			
	tok = Parser::GetNextToken(in, line);
	if (tok == ELSE) {
		ParseError(line, "Missing right brace.");
		return false;
	} else if(tok == RBRACES) {
		Parser::PushBackToken(tok);
		return true;
	} else {
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
}//End of StmtList function

//Stmt ::= AssignStme | WriteLnStmt | IfStmt  
bool Stmt(istream& in, int& line) {
	bool status = false;
	LexItem t = Parser::GetNextToken(in, line);
	switch (t.GetToken()) {
		case SIDENT: case NIDENT: 
			Parser::PushBackToken(t);
			status = AssignStmt(in, line);
			if (!status) {
				ParseError(line, "Incorrect Assignment Statement.");
				return status;
			}
			break;
		case WRITELN:
			status = WritelnStmt(in, line);
			if (!status) {
				ParseError(line, "Incorrect Writeln Statement.");
				return status;
			}
			break;
		case IF:  
			status = IfStmt(in, line);
			if (!status) {
				ParseError(line, "Incorrect If-Statement.");
				error = true;
				return status;
			}
			break;
		case ELSE:
			Parser::PushBackToken(t);
			return false;
			break;
		case IDENT:
			ParseError(line, "Invalid variable name");
			Parser::PushBackToken(t);
			return false;
			break;
		default:
			Parser::PushBackToken(t);
			return true;
	}
	return status;
}//End of Stmt function

//WritelnStmt:= WRITELN (ExpreList) 
bool WritelnStmt(istream& in, int& line) {
	ValQue = new queue<Value>;
	LexItem t = Parser::GetNextToken(in, line);
	if (t != LPAREN) {
		ParseError(line, "Missing Left Parenthesis of Writeln Statement");
		return false;
	}
	
	bool ex = ExprList(in, line);
	if (!ex) {
		ParseError(line, "Missing expression list after Print");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if (t != RPAREN) {
		ParseError(line, "Missing Right Parenthesis of Writeln Statement");
		return false;
	}

  	if	(!disable) { // delete ValQue
    	while (!(*ValQue).empty()) {
      		Value v = (*ValQue).front(); // next value in queue
      		if (v.IsInt()) cout << (int)floor(v.GetInt()) << ".0";
      		else if (v.IsString()) cout << v.GetString();
      		else if (v.IsReal()) { // deal with 50.
        		if (v.GetReal() == 50) cout << "50";
        		else {
					cout << v.GetReal();
					if (floor(v.GetReal())==v.GetReal()) cout << ".0";
				}
      		}
      		else cout << (*ValQue).front();
      		ValQue->pop();
    	}
    	cout << endl;
  	}
	return true;
}//End of WritelnStmt

//IfStmt:= IF (Expr) '{' StmtList '}' [ Else '{' StmtList '}' ] 
bool IfStmt(istream& in, int& line) {
	bool ex = false, status;
	LexItem t = Parser::GetNextToken(in, line);
	if (t != LPAREN) {
		ParseError(line, "Missing Left Parenthesis of If condition");
		return false;
	}
	Value val;
	ex = Expr(in, line, val);
	if (!ex) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
  	if (!val.IsBool()) {
    	ParseError(line, "Illegal Type for If statement condition.");
		return false;
  	}
	disable = !val.GetBool();
	
	t = Parser::GetNextToken(in, line);
	if (t != RPAREN) {
		ParseError(line, "Missing Right Parenthesis of If condition");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != LBRACES) {
		ParseError(line, "If Statement Syntax Error: Missing left brace");
		return false;
	}
	status = StmtList(in, line);
	if (!status) {
		ParseError(line, "Missing Statement for If-Stmt Clause");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RBRACES) {
		ParseError(line, "If Statement Syntax Error: Missing right brace.");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if (t == ELSE) {
		t = Parser::GetNextToken(in, line);
		if (t != LBRACES) {
			ParseError(line, "If Statement Syntax Error: Missing left brace");
			return false;
		}
    	disable = val.GetBool();
		status = StmtList(in, line);
		if (!status) {
			ParseError(line, "Missing Statement for Else-Clause");
			return false;
		}

		t = Parser::GetNextToken(in, line);
		if (t != RBRACES) {
			Parser::PushBackToken(t);
			ParseError(line, "If Statement Syntax Error: Missing right brace.");
			return false;
		}
	} else {
    	disable = false;
		Parser::PushBackToken(t);
		return true;
	}
  	disable = false;
	return true;
}//End of IfStmt function

//Var ::= NIDENT | SIDENT
bool Var(istream& in, int& line, LexItem & idtok) {
	string identstr;
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == NIDENT || tok == SIDENT) {
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second)) {
			defVar[identstr] = true;
			SymTable[identstr] = tok.GetToken();
		}
    	idtok = tok;
		return true;
	} else if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		return false;
	}
	return false;
}//End of Var

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	bool varstatus = false, status = false;
	LexItem t, idtok;
	varstatus = Var(in, line, idtok);
	Value val;
	if (varstatus) {
		t = Parser::GetNextToken(in, line);
		if (t == ASSOP) {
			status = Expr(in, line, val);
			if (!status) {
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}
		} else if (t.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		} else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	} else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}

  	if (SymTable[idtok.GetLexeme()] == NIDENT && !val.IsInt() && !val.IsReal()) {
    	ParseError(line, "Invalid assignment statement conversion of a string value to a double variable.");
    	return false;
  	} else if (val.IsBool()) { // number + boolean
    	ParseError(line, "Illegal Assignment of a boolean value to a numeric or string variable.");
    	return false;
  	} else if (!disable) ValueResults[idtok.GetLexeme()] = val;
	return status;	
}//End of AssignStmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	Value val;
	bool status = Expr(in, line, val);
	if (!status) {
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(val);
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == COMMA) status = ExprList(in, line);
	else if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	} else {
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList

//Expr ::= EqualExpr ::= RelExpr [(-EQ|==) RelExpr ]
bool Expr(istream& in, int& line, Value & retVal) {
  	Value val;
	bool t1 = RelExpr(in, line, val);
	if (!t1) return false;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if (tok == NEQ || tok == SEQ) {
    	Value next_val;
		t1 = RelExpr(in, line, next_val);
		if (!t1) {
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if (tok == NEQ) val = (val == next_val);
		else val = val.SEqual(next_val);

		if (val.IsErr()) {
			ParseError(line, "Illegal operand type for the operation.");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
  	retVal = val;
	return true;
}//End of Expr/EqualExpr

//RelExpr ::= AddExpr [ ( -LT | -GT | < | > )  AddExpr ]
bool RelExpr(istream& in, int& line, Value & retVal) {
  	Value val;
	bool t1 = AddExpr(in, line, val);
	if (!t1) return false;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	if (tok == NGTHAN || tok == NLTHAN || tok == SGTHAN || tok == SLTHAN) {
    	Value next_val;
		t1 = AddExpr(in, line, next_val);
		if (!t1) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == NGTHAN) val = (val > next_val);
		else if (tok == NLTHAN) val = (val < next_val);
		else if (tok == SGTHAN) val = val.SGthan(next_val);
		else if(tok == SLTHAN) val = val.SLthan(next_val);
		if (val.IsErr()) {
			ParseError(line, "Illegal operand type for the operation.");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
  	retVal = val;
	return true;
}//End of RelExpr

//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line, Value & retVal) {
	Value val;
	bool t1 = MultExpr(in, line, val);
	if (!t1) return false;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	
	while (tok == PLUS || tok == MINUS || tok == CAT) {
    	Value next_val;
		t1 = MultExpr(in, line, next_val);
		if (!t1) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == PLUS) val = (val + next_val);
		else if (tok == MINUS) val = (val - next_val);
		else val = val.Catenate(next_val);
		if (val.IsErr()) {
			ParseError(line, "Illegal operand type for the operation.");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
  	retVal = val;
	return true;
}//End of AddExpr

//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
bool MultExpr(istream& in, int& line, Value & retVal) {
	Value val;
	bool t1 = ExponExpr(in, line, val);
	if (!t1) return false;
	
	LexItem tok	= Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == MULT || tok == DIV  || tok == SREPEAT) {
    	Value next_val;
		t1 = ExponExpr(in, line, next_val);
		if (!t1) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == MULT) val = (val * next_val);
		else if (tok == DIV) val = (val / next_val);
		else val = val.Repeat(next_val);
		if (val.IsErr()) {
			ParseError(line, "Illegal operand type for the operation.");
			return false;
		}
		tok	= Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
  	retVal = val;
	return true;
}//End of MultExpr

//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
bool ExponExpr(istream& in, int& line, Value & retVal) {
	Value val;
	bool status = UnaryExpr(in, line, val);
	if (!status) return false;
  
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == EXPONENT) {
    	Value next_val;
		status = ExponExpr(in, line, next_val);
		if (!status) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		val = (val ^ next_val);
		tok	= Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	else if (tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	Parser::PushBackToken(tok);
  	retVal = val;
	return true;
}//End of ExponExpr

//UnaryExpr ::= ( - | + ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream& in, int& line, Value & retVal) {
	LexItem t = Parser::GetNextToken(in, line);
	int sign = 1;
	if (t == MINUS) sign = -1;
	else if (t == PLUS) sign = 1;
	else Parser::PushBackToken(t);
	Value val;
	bool status = PrimaryExpr(in, line, sign, val);
  	if ((t == MINUS || t == PLUS) && status && !val.IsInt() && !val.IsReal()) {
    	ParseError(line, "Illegal Operand Type for Sign Operator");
    	return false;
  	}
  	retVal = val;
	return status;
}//End of UnaryExpr

//PrimaryExpr ::= IDENT | NIDENT | SIDENT | ICONST | RCONST | SCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign, Value & retVal) {
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == NIDENT || tok == SIDENT) {
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second)) {
			ParseError(line, "Using Undefined Variable");
			return false;	
		}
		if (tok == NIDENT) retVal = ValueResults[lexeme] * Value(sign); // numeric variable
		else retVal = ValueResults[lexeme]; // recall value to retVal
		return true;
	}
	else if (tok == ICONST) {
		retVal = Value(sign * stoi(tok.GetLexeme()));
    	return true;
	}
	else if (tok == SCONST) {
		retVal = Value(tok.GetLexeme());
		return true;
	}
	else if (tok == RCONST) {
		retVal = Value(sign * stod(tok.GetLexeme()));
		return true;
	}
	else if (tok == LPAREN) {
    	Value out;
		bool ex = Expr(in, line, out);
		if (!ex) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if (Parser::GetNextToken(in, line) == RPAREN) {
      		if (sign == -1) out = out * (-1);
      		retVal = out;
			return ex;
    	} else {
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}
