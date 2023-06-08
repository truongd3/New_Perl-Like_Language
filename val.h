#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <sstream>

using namespace std;

enum ValType { VINT, VREAL, VSTRING, VBOOL, VERR };

class Value {
    ValType	T;
    bool    Btemp;
    int 	Itemp;
	double   Rtemp;
    string	Stemp;
    
public:
    Value() : T(VERR), Btemp(false), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(bool vb) : T(VBOOL), Btemp(vb), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(double vr) : T(VREAL), Btemp(false), Itemp(0), Rtemp(vr), Stemp("") {}
    Value(string vs) : T(VSTRING), Btemp(false), Itemp(0), Rtemp(0.0), Stemp(vs) {}
    Value(int vi) : T(VINT), Btemp(false), Itemp(vi), Rtemp(0.0), Stemp("") {}
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsString() const { return T == VSTRING; }
    bool IsReal() const {return T == VREAL;}
    bool IsBool() const {return T == VBOOL;}
    bool IsInt() const { return T == VINT; }
    
    int GetInt() const { if( IsInt() ) return Itemp; throw "RUNTIME ERROR: Value not an integer"; }
    string GetString() const { if( IsString() ) return Stemp; throw "RUNTIME ERROR: Value not a string"; }
    double GetReal() const { if( IsReal() ) return Rtemp; throw "RUNTIME ERROR: Value not an integer"; }
    bool GetBool() const {if(IsBool()) return Btemp; throw "RUNTIME ERROR: Value not a boolean";}
    void SetType(ValType type) {T = type;}
	void SetInt(int val) {Itemp = val;}
	void SetReal(double val) {Rtemp = val;}
	void SetString(string val) {Stemp = val;}
	void SetBool(bool val) {Btemp = val;}
	
    // numeric overloaded add op to this
    Value operator+(const Value& op) const {
        double oper, oper_2;
        if (GetType() == op.GetType()){
            if (IsInt()) return Value(Itemp + op.GetInt());
            else if (IsReal()) return Value(Rtemp + op.GetReal());
            else if (IsString()) {//handles both strings
                try {
                    oper = stod(GetString());
                    oper_2 = stod(op.GetString());  
                }
                catch(...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper + oper_2);            
            }
        }
        else if (IsReal() && op.IsInt()) return Value(Rtemp + (double)op.GetInt());
        else if (IsInt() && op.IsReal()) return Value((double)Itemp + op.GetReal());
        else if (op.IsString()) {
            if (!(IsReal() || IsInt())) return Value();
            try {
                oper = stod(op.GetString());
            }
            catch(...) {
                cout << "Invalid conversion from string to double." << endl;
                return Value();
            }
            if (IsInt()) return Value((double)GetInt() + oper);
            else if (IsReal()) return Value(GetReal() + oper);
        }
        else if (IsString()) {
            if (!(op.IsReal() || op.IsInt())) return Value();
            try {
                oper = stod(GetString());
            }
            catch(...){
                cout << "Invalid conversion from string to double." << endl;
                return Value();
            }
            if (op.IsInt()) return Value(oper + (double)op.GetInt());
            else if (op.IsReal()) return Value(oper + op.GetReal());
        }
        return Value();
    }
    
    // numeric overloaded subtract op from this
    Value operator-(const Value& op) const {
        double oper, oper_2;
        if (GetType() == op.GetType()){
            if (IsInt()) return Value(Itemp - op.GetInt());
            else if (IsReal()) return Value(Rtemp - op.GetReal());
            else if (IsString()) {//handles both strings
                try {
                    oper = stod(GetString());
                    oper_2 = stod(op.GetString());  
                }
                catch(...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper - oper_2);            
            }
        }
        else if (IsReal() && op.IsInt()) return Value(Rtemp - (double)op.GetInt());
        else if (IsInt() && op.IsReal()) return Value((double)Itemp - op.GetReal());
        else if (op.IsString()) {
            if (!(IsReal() || IsInt())) return Value();
            try {
                oper = stod(op.GetString());
            }
            catch(...) {
                cout << "Invalid conversion from string to double." << endl;
                return Value();
            }
            if (IsInt()) return Value((double)GetInt() - oper);
            else if (IsReal()) return Value(GetReal() - oper);
        }
        else if (IsString()) {
            if (!(op.IsReal() || op.IsInt())) return Value();
            try {
                oper = stod(GetString());
            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            if (op.IsInt()) return Value(oper - (double)op.GetInt());
            else if (op.IsReal()) return Value(oper - op.GetReal());
        }
        return Value();
    }
    
    // numeric overloaded multiply this by op
    Value operator*(const Value& op) const {
        double oper, oper_2;
        if (GetType() == op.GetType()){
            if (IsInt()) return Value(Itemp * op.GetInt());
            else if (IsReal()) return Value(Rtemp * op.GetReal());
            else if (IsString()) {//handles both strings
                try {
                    oper = stod(GetString());
                    oper_2 = stod(op.GetString());  
                }
                catch(...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper * oper_2);            
            }
        }
        else if (IsReal() && op.IsInt()) return Value(Rtemp * (double)op.GetInt());
        else if (IsInt() && op.IsReal()) return Value((double)Itemp * op.GetReal());
        else if (op.IsString()) {
            if (!(IsReal() || IsInt())) return Value();
            try {
                oper = stod(op.GetString());
            }
            catch(...) {
                cout << "Invalid conversion from string to double." << endl;
                return Value();
            }
            if (IsInt()) return Value((double)GetInt() * oper);
            else if (IsReal()) return Value(GetReal() * oper);
        }
        else if (IsString()) {
            if (!(op.IsReal() || op.IsInt())) return Value();
            try {
                oper = stod(GetString());
            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            if (op.IsInt()) return Value(oper * (double)op.GetInt());
            else if (op.IsReal()) return Value(oper * op.GetReal());
        }
        return Value();
    }
    
    // numeric overloaded divide this by op
    Value operator/(const Value& op) const {
        double oper, oper_2;
        if ((op.IsInt() && op.GetInt() == 0) || (op.IsReal() && op.GetReal() == 0.0)) {
            cout << "Illegal operand type for the operation." << endl;
            return Value();
        }
        if (GetType() == op.GetType()){
            if (IsInt()) return Value(Itemp / op.GetInt());
            else if (IsReal()) return Value(Rtemp / op.GetReal());
            else if (IsString()) {//handles both strings
                try {
                    oper = stod(GetString());
                    oper_2 = stod(op.GetString());  
                }
                catch(...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper / oper_2);            
            }
        }
        else if (IsReal() && op.IsInt()) return Value(Rtemp / (double)op.GetInt());
        else if (IsInt() && op.IsReal()) return Value((double)Itemp / op.GetReal());
        else if (op.IsString()) {
            if (!(IsReal() || IsInt())) return Value();
            try {
                oper = stod(op.GetString());
            }
            catch(...) {
                cout << "Invalid conversion from string to double." << endl;
                return Value();
            }
            if (IsInt()) return Value((double)GetInt() / oper);
            else if (IsReal()) return Value(GetReal() / oper);
        }
        else if (IsString()) {
            if (!(op.IsReal() || op.IsInt())) return Value();
            try {
                oper = stod(GetString());
            }
            catch(...){
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            if (op.IsInt()) return Value(oper / (double)op.GetInt());
            else if (op.IsReal()) return Value(oper / op.GetReal());
        }
        return Value();
    }

    //numeric overloaded equality operator of this with op
    Value operator==(const Value& op) const {
        double oper1, oper2;
        if (GetType() == op.GetType()){
            if (IsInt()) return Value(Itemp == op.GetInt());
            else if (IsReal()) return Value(Rtemp == op.GetReal());
            else if (IsString()) {//handles both strings
                try {
                    oper1 = stod(GetString());
                    oper2 = stod(op.GetString());  
                }
                catch(...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper1 == oper2);            
            }
        }
        else if (IsReal() && op.IsInt()) return Value(Rtemp == (double)op.GetInt());
        else if (IsInt() && op.IsReal()) return Value((double)Itemp == op.GetReal());
        else if (op.IsString()) {
            if (!(IsReal() || IsInt())) return Value();
            try {
                oper1 = stod(op.GetString());
            }
            catch (...) {
                cout << "Invalid conversion from string to double." << endl;
                return Value();
            }
            if (IsInt()) return Value((double)GetInt() == oper1);
            else if (IsReal()) return Value(GetReal() == oper1);
        }
        else if (IsString()) {
            if (!(op.IsReal() || op.IsInt())) return Value();
            try {
                oper1 = stod(GetString());
            }
            catch (...) {
                cout<< "Invalid conversion from string to double." <<endl;
                return Value();
            }
            if (op.IsInt()) return Value(oper1 == (double)op.GetInt());
            else if (op.IsReal()) return Value(oper1 == op.GetReal());
        }
        return Value();
    }

	//numeric overloaded greater than operator of this with op
    Value operator>(const Value& op) const {
        double oper1, oper2;
        if (GetType() == op.GetType()) {
            if (IsInt()) return Value(Itemp > op.GetInt());
            else if (IsReal()) return Value(Rtemp > op.GetReal());
            else if (IsString()){
                try {
                    oper1 = stod(GetString());
                    oper2 = stod(op.GetString());
                }
                catch(...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper1 > oper2);
            }
        }
        else if (IsInt()) {
            if (op.IsReal()) return Value(Itemp > op.GetReal());
            else if (op.IsString()) {
                try {
                    oper2 = stod(op.GetString());
                }
                catch (...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(Itemp > oper2);
            }
        }
        else if (IsString()) {
            if (op.IsInt()) {
                try {
                    oper1 = stod(GetString());
                }
                catch (...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper1 > op.GetInt());
            }
            else if (op.IsReal()) {
                try {
                    oper1 = stod(GetString());
                }
                catch (...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper1 > op.GetReal());
            }
        }
        return Value();
    }

    //numeric overloaded less than operator of this with op
    Value operator<(const Value& op) const {
        double oper1, oper2;
        if (GetType() == op.GetType()) {
            if (IsInt()) return Value(Itemp < op.GetInt());
            else if (IsReal()) return Value(Rtemp < op.GetReal());
            else if (IsString()){
                try {
                    oper1 = stod(GetString());
                    oper2 = stod(op.GetString());
                }
                catch(...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper1 < oper2);
            }
        }
        else if (IsInt()) {
            if (op.IsReal()) return Value(Itemp < op.GetReal());
            else if (op.IsString()) {
                try {
                    oper2 = stod(op.GetString());
                }
                catch (...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(Itemp < oper2);
            }
        }
        else if (IsString()) {
            if (op.IsInt()) {
                try {
                    oper1 = stod(GetString());
                }
                catch (...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper1 < op.GetInt());
            }
            else if (op.IsReal()) {
                try {
                    oper1 = stod(GetString());
                }
                catch (...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(oper1 < op.GetReal());
            }
        }
        return Value();
    }
	
    //Numeric exponentiation operation this raised to the power of op 
    Value operator^(const Value& oper) const {
        if (GetType() == oper.GetType()){
            if (IsInt()) return Value(pow(Itemp, oper.GetInt()));
            else if (IsReal()) return Value(pow(Rtemp, oper.GetReal()));
        }
        else if (IsReal() && oper.IsInt()) return Value(pow(Rtemp, (double)oper.GetInt()));
        else if (IsInt() && oper.IsReal()) return Value(pow((double)Itemp, oper.GetReal()));
        else if (IsString() || oper.IsString()) return Value();
        return Value();
    }
	
    //string concatenation operation of this with op
    Value Catenate(const Value& oper) const {
        string oper1, oper2;
        if (IsErr() || oper.IsErr()) return Value();
        if (IsString() && oper.IsString()) {
            oper1 = GetString();
            oper2 =  oper.GetString();
            return Value(oper1 + oper2);
        } else if (IsString()) {
            oper1 = GetString();
            if (oper.IsInt()) oper2 = to_string(oper.GetInt());
            else if (oper.IsReal()) oper2 = to_string(oper.GetReal());
            oper2.erase (oper2.find_last_not_of('0') + 1, string::npos);
            oper2.erase (oper2.find_last_not_of('.') + 1, string::npos );
            return Value(oper1 + oper2);
        } else if (oper.IsString()) {
            oper2 = oper.GetString();
            if (IsInt()) oper1 = to_string(GetInt());
            else if (IsReal()) oper1 = to_string(GetReal());
            oper1.erase (oper1.find_last_not_of('0') + 1, string::npos);
            oper1.erase (oper1.find_last_not_of('.') + 1, string::npos );
            return Value(oper1 + oper2);
        }
        else if (IsInt()) {
            oper1 = to_string(GetInt());
            if (oper.IsReal()) oper2 =  to_string(oper.GetReal());
            else if (oper.IsInt()) oper2 = to_string(oper.GetInt());
        }
        else if (IsReal()) {
            oper1 = to_string(GetReal());
            if (oper.IsReal()) oper2 =  to_string(oper.GetReal());
            else if (oper.IsInt()) oper2 = to_string(oper.GetInt());
        }
        oper1.erase (oper1.find_last_not_of('0') + 1, string::npos);
        oper1.erase (oper1.find_last_not_of('.') + 1, string::npos );
        oper2.erase (oper2.find_last_not_of('0') + 1, string::npos);
        oper2.erase (oper2.find_last_not_of('.') + 1, string::npos );
        return Value(oper1 + oper2);
    }

	string repeatString(string str, int times) const {//Fix maybe with const
        string repeatedStr = "";
        for (int i = 0; i < times; i++) repeatedStr += str;
        return repeatedStr;
    }
	
    //string repetition operation of this with op
    Value Repeat(const Value& oper) const {
        string oper_1 = "", oper_2 = "";
        int oper_2_2;
        if (GetType() == oper.GetType()){
            if (IsString()) {
                try {
                    oper_2_2 = stoi(oper.GetString());
                }
                catch (...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(repeatString(GetString(), oper_2_2));
            }
            else if (IsInt()) {
                try {
                    oper_1 = to_string(GetInt());
                }
                catch (...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(repeatString(oper_1,oper.GetInt()));
            }
            else if (IsReal()) {
                try {
                    oper_1 = to_string(GetReal());
                }
                catch (...) {
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(repeatString(oper_1,(int)oper.GetReal()));//Might be wrong
            }
            else return Value();
        }
        else if (IsInt()) {
            if (oper.IsReal()) {
                try{
                    oper_1 = to_string(GetInt());
                }
                catch(...){
                    cout << "Invalid conversion from string to double." << endl;
                    return Value();
                }
                return Value(repeatString(oper_1,(int)oper.GetReal()));
            }
            else if(oper.IsString()){
                try {
                    oper_1 = to_string(GetInt());
                    oper_2_2 = stoi(oper.GetString());
                }
                catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
                }
                return Value(repeatString(oper_1,oper_2_2));
            }
            else return Value();
        }
        else if(IsReal()){
            if (oper.IsInt()) {
                try {
                    oper_1 = to_string(GetReal());
                }
                catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
                }
                return Value(repeatString(oper_1, oper.GetInt()));
            }
            else if(oper.IsString()){
                try {
                    oper_1 = to_string(GetReal());
                    oper_2_2 = stoi(oper.GetString());
                }
                catch(...){
                    cout<< "Invalid conversion from string to double." <<endl;
                    return Value();
                }
                return Value(repeatString(oper_1, oper_2_2));
            }
            else return Value();
        }
        else if (IsString()) {
            if (oper.IsInt()) return Value(repeatString(GetString(), oper.GetInt()));
            else if (oper.IsReal()) return Value(repeatString(GetString(), (int)oper.GetReal()));
            else return Value();
        }
        return Value();
    }

    //string equality (-eq) operator of this with op
    Value SEqual(const Value& oper) const{
        string oper_1 = "", oper_2 = "";
        if (GetType() == oper.GetType()){//Might not work
            if (IsInt()){
                return Value(Itemp == oper.GetInt());
            }
            else if(IsReal()){
                return Value(Rtemp == oper.GetReal());
            }
            else if(IsString()){
                return Value(Stemp == oper.GetString());
            }
            else return Value();
        }
        else if(IsInt() && oper.IsReal()){
            oper_1 = to_string(GetInt());
            oper_2 = to_string(oper.GetReal());
            return Value(oper_1 == oper_2);
        }
        else if(IsReal() && oper.IsInt()){
            oper_1 = to_string(GetReal());
            oper_2 = to_string(GetInt());
            return Value(oper_1 == oper_2);
        }
        else if(IsString() && oper.IsInt()){//If op 1 is a string
            oper_2 = to_string(GetInt());
            return Value(GetString() == oper_2);
        }
        else if(IsString() && oper.IsReal()){//If op 2 is a string      
            oper_2 = to_string(oper.GetReal());
            oper_2.erase(oper_2.find_last_not_of('0') + 1, string::npos);
            oper_2.erase(oper_2.find_last_not_of('.') + 1, string::npos);
            return Value(GetString() == oper_2);
        }
        else if(IsInt() && oper.IsString()){
            oper_1 = to_string(GetInt());
            return Value(oper_1 == oper.GetString());
        }
        else if(IsReal() && oper.IsString()){
            oper_1 = to_string(GetReal());
            oper_1.erase(oper_1.find_last_not_of('0') + 1, string::npos);
            oper_1.erase(oper_1.find_last_not_of('.') + 1, string::npos);
            return Value(oper_1 == oper.GetString());
        }
        else{
            return Value();
        }
    }

    //string greater than (-gt) operator of this with op
    Value SGthan(const Value& oper) const {
        if (IsErr() || oper.IsErr()) return Value();
        string oper1, oper2;
        if (IsString() && oper.IsString()) {
            oper1 = GetString();
            oper2 =  oper.GetString();
            return Value(oper1 > oper2);
        } else if (IsString()) {
            oper1 = GetString();
            if (oper.IsInt()) oper2 = to_string(oper.GetInt());
            else if (oper.IsReal()) oper2 = to_string(oper.GetReal());
            oper2.erase (oper2.find_last_not_of('0') + 1, string::npos);
            oper2.erase (oper2.find_last_not_of('.') + 1, string::npos );
            return Value(oper1 > oper2);
        } else if (oper.IsString()) {
            oper2 = oper.GetString();
            if (IsInt()) oper1 = to_string(GetInt());
            else if (IsReal()) oper1 = to_string(GetReal());
            oper1.erase (oper1.find_last_not_of('0') + 1, string::npos);
            oper1.erase (oper1.find_last_not_of('.') + 1, string::npos );
            return Value(oper1 > oper2);
        }
        else if (IsInt()) {
            oper1 = to_string(GetInt());
            if (oper.IsReal()) oper2 =  to_string(oper.GetReal());
            else if (oper.IsInt()) oper2 = to_string(oper.GetInt());
        }
        else if (IsReal()) {
            oper1 = to_string(GetReal());
            if (oper.IsReal()) oper2 =  to_string(oper.GetReal());
            else if (oper.IsInt()) oper2 = to_string(oper.GetInt());
        }
        oper1.erase (oper1.find_last_not_of('0') + 1, string::npos);
        oper1.erase (oper1.find_last_not_of('.') + 1, string::npos );
        oper2.erase (oper2.find_last_not_of('0') + 1, string::npos);
        oper2.erase (oper2.find_last_not_of('.') + 1, string::npos );
        return Value(oper1 > oper2);
    }

    //string less than operator of this with op
    Value SLthan(const Value& oper) const {
        if (IsErr() || oper.IsErr()) return Value();
        string oper1, oper2;
        if (IsString() && oper.IsString()) {
            oper1 = GetString();
            oper2 =  oper.GetString();
            return Value(oper1 < oper2);
        } else if (IsString()) {
            oper1 = GetString();
            if (oper.IsInt()) oper2 = to_string(oper.GetInt());
            else if (oper.IsReal()) oper2 = to_string(oper.GetReal());
            oper2.erase (oper2.find_last_not_of('0') + 1, string::npos);
            oper2.erase (oper2.find_last_not_of('.') + 1, string::npos );
            return Value(oper1 < oper2);
        } else if (oper.IsString()) {
            oper2 = oper.GetString();
            if (IsInt()) oper1 = to_string(GetInt());
            else if (IsReal()) oper1 = to_string(GetReal());
            oper1.erase (oper1.find_last_not_of('0') + 1, string::npos);
            oper1.erase (oper1.find_last_not_of('.') + 1, string::npos );
            return Value(oper1 < oper2);
        }
        else if (IsInt()) {
            oper1 = to_string(GetInt());
            if (oper.IsReal()) oper2 =  to_string(oper.GetReal());
            else if (oper.IsInt()) oper2 = to_string(oper.GetInt());
        }
        else if (IsReal()) {
            oper1 = to_string(GetReal());
            if (oper.IsReal()) oper2 =  to_string(oper.GetReal());
            else if (oper.IsInt()) oper2 = to_string(oper.GetInt());
        }
        oper1.erase (oper1.find_last_not_of('0') + 1, string::npos);
        oper1.erase (oper1.find_last_not_of('.') + 1, string::npos );
        oper2.erase (oper2.find_last_not_of('0') + 1, string::npos);
        oper2.erase (oper2.find_last_not_of('.') + 1, string::npos );
        return Value(oper1 < oper2);
    }
	    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.Itemp;
		else if( op.IsString() ) out << op.Stemp ;
        else if( op.IsReal()) out << fixed << showpoint << setprecision(1) << op.Rtemp;
        else if(op.IsBool()) out << (op.GetBool()? "true" : "false");
        else out << "ERROR";
        return out;
    }
};

#endif
