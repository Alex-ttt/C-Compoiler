#pragma once
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

enum Types{
	T_INTEGER,
	T_FLOAT,
	T_IDENTIFIER,
	T_KEYWORD,
	T_SEPARATOR,
	T_OPERATION,
	T_STRING, 
	T_CHAR
};

enum Values{
	plus, minus, factor, divis, incr, 
	decr, percent, more, less, equally,
	not_eql, l_not,	b_not, xor, l_and,
	l_or, b_and, b_or,		
	more_or_eql,		// >=
	less_or_eql,		// <=
	sr,					// >>
	sl,					// <<
	assign,				// =
	pl_eql,				// +=
	mn_eql,				// -=
	fct_eql,			// *=
	div_eql,			// /=
	per_eql,			// %=
	and_eql,			// &=
	or_eql,				// |=
	xor_eql,			// ^=
	not_b_eql,			// ~=
	quest,				// ?
	sr_eql,				// >>=
	sl_eql,				// <<=
	err, 
	
	_bool,	_break,	 _case, _catch,
	_char, _const, _continue, _default,
	_delete, _do, _double, _else,
	_enum, _false, _float, _for,
	_friend, _goto,	_if, _int,
	_long, _struct, _this, _true, _typedef,
	_typename, _return, _union,
	_void, _while, _Inline,
	
	arrow,				//	->
	point,
	comma,				// ,
	semicolon,			// ;
	colon,				// :
	op_brace,			// {
	cl_brace,			// }
	op_sq_bracket,		// [	
	cl_sq_bracket,		// ]
	op_bracket,			// (
	cl_bracket,			// )

	val_int, val_float,
	val_char, val_string,
	val_ident,
	_printf,
	_scanf
};

class Token {
public:
	Values value;
	string str;
	Types type;
	int line,  start;
	Token(int l, int s, string st , Types tp ) : line(l), start(s), str(st), type(tp){}
	Token(int l, int s, int number, Types tp);
	Token(int l, int s, float number, Types tp);
	void Print() const;
	void Print(ofstream*) const;
	friend class HOptimizer;
	friend class IdentifierNode;
	virtual bool operator == (Values v) {return v == value;} 
	virtual bool operator != (Values v) {return v != value;} 
	virtual float operator = (float value) { return 0; }
	virtual int operator = (int value) { return 0; }
	bool operator == (Types t) {return t == type;} 
	bool operator != (Types t) {return t != type;} 
};

class Operation : public Token {
private:
	friend class Parser;
public:
	bool operator == (Values v) {return v == value;}
	Operation(Values v);
	bool operator != (Values v) {return v != value;}
	Operation(int l, int s, string st); 

};

class Identifier : public Token{
private:
	friend class Parser;
public:
	 Identifier(int l, int s, string st);
	 bool operator == (string v){ return v == str; }
	 bool operator != (string v) {return v != str; }
};

class KeyWord : public Token {
public:
	 KeyWord (int l, int s, string st , Types vl);
};

class IntNumber : public Token {
private:
	friend class Parser;
	friend class IntNode;
	friend class Optimizer;
	friend class BinaryOpNode;

public:
	int v2;
	IntNumber(int l, int s, string st);
	IntNumber(int l, int s, int number);
	bool operator == (int v){ return v == v2; }
	bool operator != (int v) {return v != v2; }
};

class FloatNumber : public Token {
private:
	friend class Parser;
	friend class FloatNode;
	friend class Optimizer;

public:
	float v2;
	FloatNumber(int l, int s, string st);
	FloatNumber(int l, int s, float number);
	bool operator == (float v){ return v == v2; }
	bool operator != (float v) {return v != v2; }
};

class String : public Token {
public:
	 String(int l, int s, string st , Types vl = T_STRING) : Token(l, s, st , vl){}
	 bool operator == (string v){ return v == str; }
	 bool operator != (string v) {return v != str; }
};

class Char : public Token {
private:
	friend class Parser;
	friend class CharNode;
	friend class Optimizer;
	friend class BinaryOpNode;

public:
	char v2;
	Char(int l, int s, string st) : Token(l, s, st, T_CHAR), v2(str[0]){}
	Char(int l, int s, char ch) : Token(l, s, ch, T_CHAR), v2(ch) {}
	bool operator == (char v) {return v == v2; }
	bool operator != (char v) {return v != v2; }
};

class Separator : public Token {
public:
	Separator(int l, int s, string st , Types vl);
};

class Scan {
private:
	ifstream *f;
	int line;
	int col;
	Token* t;
	char buf;
	bool end_of_file, last_token;

public:
	bool isEnd();
	Token* Get();
	bool Next();
	Scan(ifstream*);
	~Scan(void);
};

class MyException{
private:
	int line, col;
	string massage;

public:
	MyException(const string &m, int l = -1, int c = -1) : line(l), col(c), massage(m) {}
	MyException(const string &m, Token *token);
	void Print() const;
	void Print(ofstream *f) const;
};