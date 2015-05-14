#pragma once
#include "scan.h"
#include "generator.h"
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;


enum S_types{
	t_Int,
	t_Float,
	t_Char,
	t_Void,
	t_Struct,
	t_Error
};

class SymbolType;

class Symbol{
public:
	string name;
	int offset;
	Symbol(){}
	Symbol(string n) : name(n), offset(0){}
	virtual SymbolType *getType() { return 0; }
	virtual int byteSize() const { return 0; }
	virtual bool isFuncVar() const { return false; }
	virtual bool isFunc() const { return false; }
	virtual void generate(AsmCode &code) const {}
	virtual void print(int deep) const;
	virtual Symbol* SelfCopy() { return 0; }
};

class SymbolType : public Symbol{
public:
	S_types s_type;
	bool Inline;
	SymbolType(string n, bool In = false, S_types t = t_Void) : Symbol(n), Inline(In) { s_type = t; }
	virtual string typeName() const;
	virtual bool isStruct() { return false; }
	virtual bool isPointerSymbol() const { return false; }
	virtual bool isArraySymbol() const { return false; }
	virtual SymbolType* upType() {return 0; }
	virtual bool canConvertTo(SymbolType *to) { return false; }
	virtual SymbolType* getType() { return this; }
	virtual void initType(SymbolType *t) {}
	virtual bool GetInline() const { return Inline; }
	void SetInline(bool ok) { Inline = ok; }
	virtual bool isModifiableLvalue() const { return false; }
	bool operator == (const string &n) const { return n == name; }
	bool operator != (const string &n) const { return n != name; }
	virtual bool operator == (SymbolType *t) const { return this == t; }
	virtual bool operator != (SymbolType *t) const { return !(this == t); }
	virtual bool isConstSymbol() const { return false; }
}; 

class ScalarSymbol: public SymbolType{
public:
	ScalarSymbol(const string &name, S_types t = t_Void) : SymbolType(name,t) {}
	bool canConvertTo(SymbolType* to);
	bool isModifiableLvalue() const;
	int byteSize() const;
	Symbol* SelfCopy() { return new ScalarSymbol(name); } // Исправить
};

class Block;
class Node;

class SymTable : public Symbol{
public:	
	int offset;
	int shift;
	vector<string> name;
	vector<Symbol*> sym_ptr;
	map <string, int> index;
	vector<Node*> callParams;
	
	SymTable(int tableShift = 0) : name(0), sym_ptr(0), offset(0), shift(tableShift), callParams(0) {}
	void print(int deep = 0) const;
	bool isExist(const string &name) const;
	virtual void add_symbol(Symbol *s);
	bool exists(const string &name) const;;
	void generateGlobals(AsmCode &code) const;
	void generateCode(AsmCode &code) const;
	bool operator == (SymTable *t) const;
	bool operator != (SymTable *t) const; 
	Symbol* operator [] (int) const;
	Symbol* find_symbol(const string &name) const;
	Symbol* find_type(const string &name) const;
	//Symbol* SelfCopy();
	void CreateCallParams();
	void InitCallParams(vector<Node*> &params, Block *block);
	Symbol* back() const;
	int byteSize() const;
	int size() const;
};

class SymTableForLocals : public SymTable{
public:
	SymTableForLocals(int off = 0) : SymTable(off) {}
	void add_symbol(Symbol *s);
};

class SymTableForParams : public SymTable{
public:
	SymTableForParams() : SymTable() { offset = 4; }
	void add_symbol(Symbol *s);
};

class SymTableForFields : public SymTable{
public:
	SymTableForFields() : SymTable() {}
	void add_symbol(Symbol *s);
};

class SymTableStack : public Symbol{
private:
	friend class Parser;
	vector <SymTable*> tables;

public:
	void add_symbol(Symbol* s);
	SymTable* top();
	bool exist_in_top(const string &name);
	void push(SymTable* t);
	void pop();
	void print(int deep = 0) const;
	Symbol* find_symbol(const string &name) const; 
	Symbol* find_type(const string &name) const;
	int size() const;
};

extern Node* init_var(SymbolType *type);

class VarSymbol : public SymbolType{
public:
	Node *init;
	int assignCounter;
	Node *lastUsing;
	bool global;
	SymbolType *type;
	SymbolType *getType();
	VarSymbol(const string &str, SymbolType *tp, Node *i = 0);
	void print(int deep) const;
	bool GetInline() const;
	bool isFuncVar() const;
	void initType(SymbolType *t) { type = t; }
	void generate(AsmCode &code) const;
	int byteSize() const;
};

class ConstSymbolType : public SymbolType{
public:	
	SymbolType *symbol;
	ConstSymbolType(SymbolType *symb, S_types s_type = t_Void) : SymbolType("const_" + symb->name, s_type), symbol(symb) {}
	string typeName() const;
	bool canConvertTo(SymbolType* to);
	bool isConstSymbol() const { return true; }
	virtual bool isStruct() { return symbol->isStruct(); }
};

class StructSymbol : public SymbolType{
public:
	SymTable *s_table;
	StructSymbol(SymTable *st, string name) : SymbolType(name, t_Struct), s_table(st) {}
	void print(int deep) const;
	SymbolType* upType();
	string typeName() const;
	int byteSize() const; 
	int getShiftForBase() const;
	virtual bool isStruct() { return true; }
	bool canConvertTo(SymbolType* to);
};	

class Block;
class PointerSymbol;

class ArraySymbol : public SymbolType{
public:
	int size;
	SymbolType *type;
	ArraySymbol(SymbolType *st, int sz, const string &name = "") : SymbolType("array"), size(sz), type(st) {}
	string typeName() const;
	SymbolType* upType() {return type; }
	bool canConvertTo(SymbolType *to);
	bool operator == (SymbolType *s) const;
	bool operator != (SymbolType *s) const;
	bool isArraySymbol() const { return true; }
	void initType(SymbolType *t) { type = t; }
	int byteSize() const;
	PointerSymbol* convertToPointer() const;
};

class FuncSymbol : public SymbolType{
public:
	SymTable *params;
	SymbolType *value;
	Block *body;
	AsmArgLabel *endLabel;

	FuncSymbol(const string &name, SymbolType *val, Block *block = 0);
	bool operator == (SymbolType *s) const;
	bool isFunc() const;
	bool operator != (SymbolType *s) const;
	void generate(AsmCode &code, const string &str) const;
	void print(int deep) const;
	bool UseRecursion() const;
	SymbolType *getType();
	bool canConvertTo(SymbolType* to);
	void initType(SymbolType *t) {value = t; }
};

class PointerSymbol : public SymbolType{
public:
	SymbolType *pointer;
	PointerSymbol(SymbolType *ptr) : SymbolType("", ptr->Inline), pointer(ptr) {}
	SymbolType* upType() { return pointer; }
	string typeName() const;
	bool canConvertTo(SymbolType* to);
	bool isModifiableLvalue() const;
	bool operator == (SymbolType *t) const;
	bool operator != (SymbolType *t) const;
	void initType(SymbolType *t) { pointer = t; }
	bool isPointerSymbol() const { return true; }
	int byteSize() const;
};

extern ScalarSymbol *IntType;
extern ScalarSymbol *FloatType;
extern ScalarSymbol *CharType;
extern ScalarSymbol *VoidType;
extern PointerSymbol *StringType;

extern map<SymbolType*, int> typePriority;
extern map<Values, SymbolType*> operationTypeOperands;
extern map<Values, SymbolType*> operationReturningType;