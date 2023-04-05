#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <list>
#include <algorithm>
#include "lex.h"
using namespace std;

// for id_or_kw method
static bool isNumberIDENT(string str) {
    if (str[0] != '$') return false;
    if (str.length() < 2) return false;
    for (int i = 1; i < str.length(); i++) {
        if (!(isalpha(str[i]) || isdigit(str[i]) || str[i] == '_')) return false;
    }
    return true;
}

static bool isStringIDENT(string str) {
    if (str[0] != '@') return false;
    if (str.length() < 2) return false;
    for (int i = 1; i < str.length(); i++) {
        if (!(isalpha(str[i]) || isdigit(str[i]) || str[i] == '_')) return false;
    }
    return true;
}

static string lowercase(string str) {
    string res = "";
    for (char letter : str) res += tolower(letter);
    return res;
}

LexItem id_or_kw(const string& lexeme, int linenum) {
    if (isNumberIDENT(lexeme) == true) {
        LexItem res(NIDENT, lexeme, linenum); // number identifiers
        return res;
    } else if (isStringIDENT(lexeme) == true) {
        LexItem res(SIDENT, lexeme, linenum); // sting identifiers
        return res;
    } else if (lowercase(lexeme) == "if") {
        LexItem res(IF, lexeme, linenum);
        return res;
    } else if (lexeme == "else") {
        LexItem res(ELSE, lexeme, linenum);
        return res;
    } else if (lexeme == "writeln") {
        LexItem res(WRITELN, lexeme, linenum);
        return res;
    } else {
        LexItem res(IDENT, lexeme, linenum);
        return res;
    }
}

// for operator<<
static string toStringg(Token tok) {
    if (tok == WRITELN) return "WRITELN";
    else if (tok == IF) return "IF";
    else if (tok == ELSE) return "ELSE";
    else if (tok == IDENT) return "IDENT";
    else if (tok == NIDENT) return "NIDENT";
    else if (tok == SIDENT) return "SIDENT";
    else if (tok == ICONST) return "ICONST";
    else if (tok == RCONST) return "RCONST";
    else if (tok == SCONST) return "SCONST";
    else if (tok == PLUS) return "PLUS";
    else if (tok == MINUS) return "MINUS";
    else if (tok == MULT) return "MULT";
    else if (tok == DIV) return "DIV";
    else if (tok == EXPONENT) return "EXPONENT";
    else if (tok == ASSOP) return "ASSOP";
    else if (tok == LPAREN) return "LPAREN";
    else if (tok == RPAREN) return "RPAREN";
    else if (tok == LBRACES) return "LBRACES";
    else if (tok == RBRACES) return "RBRACES";
    else if (tok == NEQ) return "NEQ";
    else if (tok == NGTHAN) return "NGTHAN";
    else if (tok == NLTHAN) return "NLTHAN";
    else if (tok == CAT) return "CAT";
    else if (tok == SREPEAT) return "SREPEAT";
    else if (tok == SEQ) return "SEQ";
    else if (tok == SLTHAN) return "SLTHAN";
    else if (tok == SGTHAN) return "SGTHAN";
    else if (tok == COMMA) return "COMMA";
    else if (tok == SEMICOL) return "SEMICOL";
    else if (tok == ERR) return "Error";
    else return "DONE";
}

ostream& operator<<(ostream& out, const LexItem& tok) {
    string tok_res = toStringg(tok.GetToken());
    string lexeme = tok.GetLexeme();
    int linenumber = tok.GetLinenum();

    if (tok_res == "SCONST") out << tok_res << "(" << lexeme.substr(1, lexeme.size() - 2) << ")" << endl;
    else if (tok_res == "Error") out << "Error in line " << linenumber << " (" << lexeme << ")" << endl;
    else if (tok_res.find("CONST") != -1 || tok_res.find("IDENT") != -1) out << tok_res << "(" << lexeme << ")" << endl;
    else out << tok_res << endl;
    return out;
}

//a segment of the getNextToken code
LexItem getNextToken(istream& in, int& linenum) {
    enum TokState {
        START, 
        INID, // recognize identifiers IDENT --> done
        ININT, // recognize integer ICONST
        INREAL, // recognize real RCONST
        INSTRING, // recognize string SCONST
        INOP, // recognize operators
        INCOMMENT // recognize comment
    };
    enum TokState lexstate = START;
    string lexeme = "";
    char ch;
    while (in.get(ch)) {
        switch (lexstate) {
            case START:
                if (ch == '\n') linenum += 1; // new line
                else if (isspace(ch)) continue; // BLANK SPACE
                else if (isalpha(ch) || ch == '_' || ch == '@' || ch == '$') {
                    lexstate = INID; // recognize identifiers
                    lexeme += ch;
                }
                else if (isdigit(ch)) { // recognize number ININT / INREAL
                    lexstate = ININT;
                    lexeme += ch;
                }
                else if (ch == '\''){ // recognize string (single quote)
                    lexstate = INSTRING;
                    lexeme += ch;
                }
                else if (ch == '#') lexstate = INCOMMENT; // recognize INCOMMENT
                else if (ch == '-') { // MINUS or 3 OPs
                    lexstate = INOP;
                    lexeme += ch;
                }
                else {
                    Token tok = ERR;
                    switch (ch) {
                        case '+':
                            tok = PLUS;
                            break;
                        case '*': // 2 situations
                            if (in.peek() == '*') {
                                tok = SREPEAT;
                                lexeme += in.get();
                            } else tok = MULT;
                            break;
                        case '/':
                            tok = DIV;
                            break;
                        case '^':
                            tok = EXPONENT;
                            break;  
                        case '=': // 2 situations
                            if (in.peek() == '=') {
                                tok = NEQ;
                                lexeme += in.get();
                            } else tok = ASSOP;
                            break;
                        case '>':
                            tok = NGTHAN;
                            break;
                        case '<':
                            tok = NLTHAN;
                            break;
                        case '.':
                            tok = CAT;
                            break;
                        case ';':
                            tok = SEMICOL;
                            break;
                        case ',':
                            tok = COMMA;
                            break;
                        case '(':
                            tok = LPAREN;
                            break;
                        case ')':
                            tok = RPAREN;
                            break;
                        case '[':
                            tok = LBRACES;
                            break;
                        case ']':
                            tok = RBRACES;
                            break;
                        case '{':
                            tok = LBRACES;
                            break;
                        case '}':
                            tok = RBRACES;
                            break;
                        default:
                            lexeme += ch;
                            return LexItem(ERR, lexeme, linenum);
                            break;
                    }
                    return LexItem(tok, lexeme, linenum);
                }
                break;

            case INID:
                if (isalpha(ch) || isdigit(ch) || ch == '_') lexeme += ch;
                else {
                    lexstate = START;
                    in.putback(ch);
                    return id_or_kw(lexeme, linenum);
                }
                break;

            case ININT:
                if (isdigit(ch)) lexeme += ch;
                else if (ch == '.' && isdigit(in.peek()) ) {
                    lexstate = INREAL;
                    lexeme += ch;
                } else if (ch == '.' && !isdigit(in.peek())) {
                    lexeme += ch;
                    return LexItem(RCONST, lexeme, linenum);
                } else {
                    lexstate = START;
                    in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                }
                break;

            case INREAL:
                if (isdigit(ch)) lexeme += ch;
                else if (ch == '.') { // testcase realerr
                    if (isdigit(in.peek())) {
                        lexeme += ch;
                        return LexItem(ERR, lexeme, linenum);
                    } else {
                        in.putback(ch);
                        return LexItem(RCONST, lexeme, linenum);
                    }
                } else {
                    in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                }
                break;

            case INSTRING:
                if (ch == '\'') {
                    lexeme += ch;
                    return LexItem(SCONST, lexeme, linenum);
                } else if (ch == '\n') {
                    in.putback(ch);
                    return LexItem(ERR, lexeme, linenum);
                } else lexeme += ch;
                break;

            case INOP:
                if (tolower(ch) == 'e' && tolower(in.peek()) == 'q') { // SEQ: -eq
                    lexeme += ch + in.get();
                    return LexItem(SEQ, lexeme, linenum);
                } else if (tolower(ch) == 'l' && tolower(in.peek()) == 't') { // SLTHAN: -lt
                    lexeme += ch + in.get();
                    return LexItem(SLTHAN, lexeme, linenum);
                } else if (tolower(ch) == 'g' && tolower(in.peek()) == 't') { // SGTHAN: -gt
                    lexeme += ch + in.get();
                    return LexItem(SGTHAN, lexeme, linenum);
                } else {
                    in.putback(ch);
                    return LexItem(MINUS, lexeme, linenum);
                }
                break;

            case INCOMMENT:
                if (ch == '\n') {
                    linenum++;
                    lexstate = START;
                    in.get();
                }
                break;
        }
    }

    if (in.eof()) return LexItem(DONE, "", linenum);
    else {
        lexeme += ch;
        return LexItem(ERR, "Error", linenum);
    }
}

// for main
static double toNumber(string str) {
    double result = 0, decimal = 10;
    bool check = 0;
    for (char letter : str) {
        if (letter == '.') check = 1;
        else if (check == 1) {
            result += (letter - '0') / decimal;
            decimal *= 10;
        } 
        else if (check == 0) result = result * 10 + (letter - '0');
    }
    return result;
}

int main(int argc, char *argv []) {
    // check input from command line
    bool v = false, nconst = false, sconst = false, ident = false;
    bool hasFile = false;
    ifstream file;

    for (int i = 1; i < argc; i++){
        string var = argv[i];
        if (var == "-v") v = !v;
        else if (var == "-nconst") nconst = !nconst;
        else if (var == "-sconst") sconst = !sconst;
        else if (var == "-ident") ident = !ident;
        else if (var[0] == '-') { // flag is not recognized
            cerr << "UNRECOGNIZED FLAG {" << argv[i] << "}" << endl;
            return -1; // stop running
        }
        else { // check file name finally
            if (hasFile) { // has 1+ files
                cerr << "ONLY ONE FILE NAME IS ALLOWED." << endl;
                return -1; // stop running
            } else {
                hasFile = true;
                file.open(argv[i]);
                if (!file) { // cannot open file name
                    cerr << "CANNOT OPEN THE FILE " << argv[i] << endl;
                    return -1;
                }
            }
        }
    }

    if (hasFile == false) { // no file is proved
        cerr << "NO SPECIFIED INPUT FILE." << endl;
        return -1; // stop running
    } else if (file.peek() == EOF) { // file is empty
        cerr << "Lines: 0" << endl;
        cerr << "Empty File." << endl;
        return 0;
    }

    list<string> idents;
    list<double> numbers;
    list<string> strings;
    int lineNumber = 1, countToken = 0;
    LexItem tok;
    while ((tok = getNextToken(file, lineNumber)) != DONE) { 
        countToken++;

        if (v) cout << tok;
        if (tok.GetToken() == ERR) return -1;

        if (tok.GetToken() == IDENT || tok.GetToken() == NIDENT || tok.GetToken() == SIDENT){
            if (find(idents.begin(), idents.end(), tok.GetLexeme()) == idents.end()) idents.push_back(tok.GetLexeme());
        }
        else if (tok.GetToken() == ICONST || tok.GetToken() == RCONST) {
            double resultLexeme = toNumber(tok.GetLexeme());
            if (find(numbers.begin(), numbers.end(), resultLexeme) == numbers.end()) numbers.push_back(resultLexeme);
        }
        else if (tok.GetToken() == SCONST) {
            if (find(strings.begin(), strings.end(), tok.GetLexeme()) == strings.end()) strings.push_back(tok.GetLexeme());
        }
    }
    cout << endl;

    cout << "Lines: " << lineNumber - 1 << endl;
    cout << "Total Tokens: " << countToken << endl;
    cout << "Identifiers: " << idents.size() << endl;
    cout << "Numbers: " << numbers.size() << endl;
    cout << "Strings: " << strings.size() << endl;

    if (ident && idents.size() > 0) {
        cout << "IDENTIFIERS:" << endl;
        idents.sort();
        int count = 0, limit = idents.size() - 1;
        for (string temp : idents) {
            if (count < limit) {
                count++;
                cout << temp << ", ";
            } else cout << temp << endl;
        }
    }

    if (nconst && numbers.size() > 0) {
        cout << "NUMBERS:" << endl;
        numbers.sort();
        for (double temp : numbers) cout << temp << endl;
    }

    if (sconst && strings.size() > 0) {
        cout << "STRINGS:" << endl;
        strings.sort();
        for (string temp : strings) cout << temp << endl;
    }

    return 0;
}
