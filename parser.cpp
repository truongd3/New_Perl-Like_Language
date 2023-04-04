#include "parser.h"

map<string, bool> defVar;
map<string, Token> SymTable;

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

int ErrCount() {
    return error_count;
}

void ParseError(int line, string msg) {
	++error_count;
	cout << error_count << ". Line # " << line << ": " << msg << endl;
}

// WritelnStmt:= WRITELN (ExpreList) 
bool WritelnStmt(istream& in, int& line) {
	LexItem t;
	
	t = Parser::GetNextToken(in, line);
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

	return true;
} //End of WritelnStmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = Expr(in, line);
	if (!status) {
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == COMMA) status = ExprList(in, line);
	else {
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
} //End of ExprList

//Prog ::= StmtList
bool Prog(istream& in, int& line) {
	bool progstatus = StmtList(in, line);
	if (!progstatus) {
		ParseError(line, "Missing Program");
		return false;
	} else if (progstatus) {
		cout << "(DONE)" << endl;
		return true;
	}
    return progstatus;
}

//Stmt ::= AssignStmt | WriteLnStmt | IfStmt
bool Stmt(istream& in, int& line) {
	LexItem t = Parser::GetNextToken(in, line);
	switch (t.GetToken()) {
		case IF:
			if (IfStmt(in, line) == false) {
				ParseError(line, "Incorrect If-Statement.");
				return false;
			}
			else return true;
			break;
		case SIDENT: case NIDENT:
			Parser::PushBackToken(t);
			if (AssignStmt(in, line) == false) {
				ParseError(line, "Incorrect Assignment Statement.");
				return false;
			}
			else return true;
			break;
		case WRITELN:
			if (WritelnStmt(in, line) == false) {
				ParseError(line, "Incorrect Writeln Statement.");
				return false;
			}
			else return true;
			break;
		default:
			Parser::PushBackToken(t);
			return false;
	}
	return true;
}

//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line){
    bool status = Stmt(in, line);
    if (status){
        LexItem t = Parser::GetNextToken(in, line);
		while (t == SEMICOL) {
			status = Stmt(in, line);
			if (!status) break;
			else t = Parser::GetNextToken(in, line);
		}
		if (status && t != SEMICOL) {
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		} else if (!status && ErrCount() > 0) {
			ParseError(line, "Syntactic error in Program Body.");
			return false;
		}
    } else {
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
    return true;
}

//IfStmt ::= if (Expr) ‘{‘ StmtList ‘}’ [ else ‘{‘ StmtList ‘}’ ]
bool IfStmt(istream& in, int& line) {
	LexItem t = Parser::GetNextToken(in, line);
	if (t != LPAREN) {
		ParseError(line, "Missing Left Parenthesis of If condition");
		return false;
	}

	bool ex = ExprList(in, line);
	if (!ex) {
		ParseError(line, "Missing Expression");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN) {
		ParseError(line, "Missing Right Parenthesis of If condition");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != LBRACES) {
		ParseError(line, "Missing Left Brace of If condition.");
		return false;
	}

	bool statement = StmtList(in, line);
	if (!statement) {
		ParseError(line, "Missing Statement for If-Stmt Clause");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RBRACES) {
		ParseError(line, "Missing Right Brace of If condition.");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t.GetToken() != ELSE) Parser::PushBackToken(t); // push back if not ELSE
	else {
		t = Parser::GetNextToken(in, line);
		if (t.GetToken() != LBRACES) {
			ParseError(line, "Missing Left Brace of Else condition.");
			return false;
		}

		statement = StmtList(in, line);
		if (!statement) {
			ParseError(line, "Missing Statement for Else Clause");
			return false;
		}

		t = Parser::GetNextToken(in, line);
		if (t.GetToken() != RBRACES) {
			ParseError(line, "Missing Right Brace of Else condition.");
			return false;
		}
	}
    return true;
}

//AssignStmt ::= Var = Expr
bool AssignStmt(istream& in, int& line){
    bool status = false, varStatus = Var(in, line);
    if (varStatus) {
        LexItem t = Parser::GetNextToken(in, line); 
		string varStr = t.GetLexeme(); // get variable
        if (t.GetToken() == ASSOP) { // detect =
            status = Expr(in, line);
            if (!status) {
                ParseError(line, "Missing Expression in Assignment Statement");
                return false;
            } else defVar[varStr] = true; // add to map, variable has been assigned to a value
        } else {
            ParseError(line, "Missing Assignment Operator =");
            return false;
        }
    } else {
        ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
        return false;
    }
    return status;
}

//Var ::= NIDENT | SIDENT
bool Var(istream& in, int& line) {
    LexItem t = Parser::GetNextToken(in, line);
    if (t == NIDENT || t == SIDENT){
		defVar[t.GetLexeme()] = true; // variable is initialized but has no value
        return true;
    } else {
        ParseError(line, "Unrecognized Variable Type");
        return false;
    }
    return true;
}

//Expr ::= RelExpr [ ( -eq | == ) RelExpr ]
bool Expr(istream& in, int& line) {
	//cout << "in Expr function" << endl;
	bool status = RelExpr(in, line);
	if (!status) {
		ParseError(line, "Missing Relationship Expression.");
		return false;
	}

	LexItem t = Parser::GetNextToken(in, line);
	if (t.GetToken() == SEQ || t.GetToken() == NEQ) {
		status = RelExpr(in, line);
		if (!status) {
			ParseError(line, "Missing RelExpr after Equal.");
			return false;
		} else return true;
	}
	else Parser::PushBackToken(t);
    return true;
}

//RelExpr ::= AddExpr [ ( -lt | -gt | < | > ) AddExpr ]
bool RelExpr(istream& in, int& line){
    bool status = AddExpr(in, line);
	if (!status) {
		ParseError(line, "Missing Add Expr in Rel Expr.");
		return false;
	}

    LexItem t = Parser::GetNextToken(in, line);
    if (t == NLTHAN || t == NGTHAN || t == SLTHAN || t == SGTHAN) {
		status = AddExpr(in, line);
		if (!status) {
			ParseError(line, "Missing Add Expr after Relationship Operator.");
			return false;
		} else return true;
	}
    else Parser::PushBackToken(t);
    return true;
}

//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line){
    bool status = MultExpr(in, line);
    if (!status) {
		ParseError(line, "Missing MultExpr in AddExpr.");
		return false;
	} 

    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == PLUS || t.GetToken() ==  MINUS || t.GetToken() == CAT) {
        status = MultExpr(in, line);
        if (!status) {
            ParseError(line, "Missing operand after operator");
            return false;
        }
    }
    else Parser::PushBackToken(t);
    return true;
}

//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
bool MultExpr(istream& in, int& line){
    bool status = ExponExpr(in, line);
    if (!status) {
		ParseError(line, "Missing Expon Expr in Mult Expr.");
		return false;
	} 

    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == MULT || t.GetToken() ==  DIV || t.GetToken() == SREPEAT) {
        status = ExponExpr(in, line);
        if (!status) {
            ParseError(line, "Missing operand after operator");
            return false;
        }
    }
    else Parser::PushBackToken(t);
    return true;
}

//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
bool ExponExpr(istream& in, int& line) {
	bool status = UnaryExpr(in, line);
    if (!status) {
		ParseError(line, "Missing Unary Expr in Expon Expr.");
		return false;
	}

    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == EXPONENT) {
        status = UnaryExpr(in, line);
        if (!status) {
            ParseError(line, "Missing Unary Expr after exponent operator");
            return false;
        }
    }
    else Parser::PushBackToken(t);
	return true;
}

//UnaryExpr ::= [( - | + )] PrimaryExpr
bool UnaryExpr(istream& in, int& line){
    int sign = 0;
	LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == MINUS) sign = -1;
    else if (t.GetToken() == PLUS) sign = 1;
    else Parser::PushBackToken(t);
    return PrimaryExpr(in, line, sign);
}

//PrimaryExpr ::= IDENT | SIDENT | NIDENT | ICONST | RCONST | SCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign){
    LexItem t = Parser::GetNextToken(in, line);
    if (t != IDENT && t != SIDENT && t != NIDENT && t != ICONST && t != RCONST && t != SCONST) {
		if (t.GetToken() != LPAREN) {
			ParseError(line, "Missing Left Parenthesis.");
			return false;
		}

		bool ex = Expr(in, line);
		if (!ex) {
			ParseError(line, "Missing expression after parenthesis.");
			return false;
		}

		t = Parser::GetNextToken(in, line);
		if (t.GetToken() != RPAREN) {
			ParseError(line, "Missing Right Parenthesis.");
			return false;
		}
	}
	if (t == IDENT || t == SIDENT || t == NIDENT) {
		if (!(defVar.find(t.GetLexeme())->second) || defVar[t.GetLexeme()] == false) {
            ParseError(line, "Using Undefined Variable");
            return false;
        }
	}
    return true;
}