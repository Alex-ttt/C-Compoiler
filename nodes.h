#pragma once

#include <vector>

#include "symbols.h"

class FuncCallNode;

class Node{
protected:
	Token *token;
	void generateST0ToStack(AsmCode &code);	
	void generateByteToFPU(AsmCode &code);
	
	
public:
	SymbolType *nodeType;
	Node *replaced;
	Node *commonNode;
	bool opt;
	bool needDeleteng;
	int index;
	friend class Parser;
	friend class FunctionalNode;
	friend class BinaryOpNode;
	friend class IdentifierNode;
	friend class SymTable;
	
	Node();
	Node(Token* t);
	virtual void print(int deep = 0) const {};
	virtual void generate(AsmCode &code) {}
	virtual void generateLvalue(AsmCode& code) {}
	virtual void generateFPUStack(AsmCode& code){}
	virtual void print(ofstream *t, int deep = 0) const {}
	virtual void ReplaceCallNode(FuncCallNode *what, Node *to) {}
	virtual SymbolType* getType() { return 0; }
	virtual bool isLvalue() const { return false; }
	Values value() const { return token->value; }
	virtual bool isModifiableLvalue() const { return false; }
	static Node* makeTypeCoerce(Node* expr, SymbolType *from, SymbolType *to);
	virtual bool isBinaryOpNode() { return false; }
	virtual vector<FuncCallNode*>* GetInlineFunction() { return 0; };
	virtual bool isUnaryOpNode () const { return false; }
	virtual bool isConst() { return false; }
	virtual bool isSimple() const {return false; }
	virtual bool isIdent() const { return false; }
	virtual bool isCycle() const {return false; }
	virtual bool isString() const { return false; }
	virtual bool isBlock() const { return false; }
	virtual bool isExpression() const { return true; }
	virtual bool isIfStatement() const {return false; }
	virtual bool isTernaryOpNode() const { return false; }
	virtual bool isReturnExpression() const { return false; }
	virtual bool ContainLvalueRecourse(VarSymbol *id) const { return false; }
	virtual bool operator == (int val) { return false; }
	virtual bool operator == (Node *node) const { return false; }
	virtual int operator = (int val) { return val; }
	virtual void ReplaceDiv2Mult() {}
	virtual Node* SelfCopy() { return 0; }
	virtual void generateLoadInFPUStack(AsmCode& code) const {}
	virtual void SetIndex(int i) { index = i; }
	virtual Node* calculate() { return 0; }
	friend class ArrNode;
	friend class TernaryOpNode;
	friend class CastNode;
	friend class UnaryOpNode;
	friend class FuncCallNode;
};

class EmptyNode : public Node{
public:
	EmptyNode(): Node(0) {}
	void print(int deep) const {};
	void generate(AsmCode& code) const {}
};

class Block;

class GoToNode : public Node{
private:
	Block *block;
	AsmArgLabel *label;

public:
	GoToNode(Block *b);
	GoToNode(AsmArgLabel *jump);
	void generate(AsmCode &code) const;
	void print(int deep) const;
};

class OpNode : public Node{
protected:
	static string opName(Values v);
public:
	OpNode(Token *op);
	virtual void print(int deep) const {}
	virtual void print(int deep, ofstream* f) const {}
};

class BinaryOpNode : public OpNode{
protected:
	Node *left;
	Node *right;

public:
	friend class Parser;
	friend class VarSymbol;
	friend class SubExpressionTap;
	friend class HOptimizer;
	friend class InlineCall;
	BinaryOpNode(Token *op, Node *l, Node *r);
	virtual SymbolType *getType();
	bool isLvalue() const;
	bool isModifiableLvalue() const;
	void print(int deep) const;
	void print(ofstream *f, int deep);
	void generate(AsmCode &code);
	void generateForFloat(AsmCode &code);
	void generateLvalue(AsmCode& code);
	void generateFPUStack(AsmCode& code);
	void SetIndex(int i);
	void ReplaceCallNode(FuncCallNode *what, Node *to);
	vector<FuncCallNode*>* GetInlineFunction();
	void ReplaceDiv2Mult();
	Node* SelfCopy();
	Node* calculate();
	bool operator == (Node *node) const;
	bool ContainLvalueRecourse(VarSymbol *id) const;
	bool Associative() const;
	static bool isAssignment(Values op);
	static bool isComparison(Values op);
	virtual bool isBinaryOpNode() { return true; }
	bool isConst() const;
};

class GoToBinaryOpNode : public BinaryOpNode{
private:
	AsmArgLabel *label;

public:
	GoToBinaryOpNode(Token *op, Node *l, Node *r, AsmArgLabel *jump) : BinaryOpNode(op, l, r), label(jump) {}
	void generate(AsmCode &code);
	void print(int deep) const;
};

class UnaryOpNode : public OpNode{
protected:
	Node *operand;
public:
	UnaryOpNode(Token *t, Node *op);
	void print(int deep) const;
	void print(ofstream *f, int deep) const;
	SymbolType *getType();
	bool isLvalue() const;
	void ReplaceDiv2Mult();
	bool isModifiableLvalue() const;
	virtual void generate(AsmCode &code);
	void ReplaceCallNode(FuncCallNode *what, Node *to);
	vector<FuncCallNode*>* GetInlineFunction();
	void SetIndex(int i);
	Node* SelfCopy();
	bool isUnaryOpNode () const { return true; }
	void generateFPUStack(AsmCode& code);
	void generateLvalue(AsmCode& code);
	bool ContainLvalueRecourse(VarSymbol *id) const;
	bool operator == (Node *node) const;
	Node* calculate();
	virtual bool isConst() const;
	friend class HOptimizer;
	friend class SubExpressionTap;
	friend class InlineCall;
};

class PostfixUnaryOpNode : public UnaryOpNode{
public:
	PostfixUnaryOpNode(Token *t, Node *op);
	void print(int deep) const;
	void print(ofstream *f, int deep) const;
	bool isLvalue() const;
	void generate(AsmCode &code);
	bool isModifiableLvalue() const;
};

class CastNode : public UnaryOpNode{
private:
	SymbolType *s_type;

public:
	CastNode(Token *op, Node *oper, SymbolType *ts);
	virtual SymbolType *getType();
	Node* calculate();
	void generateFPUStack(AsmCode& code);
	void print(int deep) const;
	void print(ofstream *f, int deep) const;
};

class RetNode : public UnaryOpNode{
public:
	RetNode(Token *token, Node *n) : UnaryOpNode(token, n) {}
	bool isReturnExpression() const { return true; }
	void print(int deep) const;
};

class TernaryOpNode : public BinaryOpNode{
private:
	Node* condition;
public:
	TernaryOpNode(Token* op, Node* c, Node* l, Node* r);
	void print(int deep) const;
	void print(ofstream *f, int deep) const;
	void ReplaceDiv2Mult();
	Node* SelfCopy();
	void ReplaceCallNode(FuncCallNode *what, Node *to);
	vector<FuncCallNode*>* GetInlineFunction();
	Node* calculate();
	bool isTernaryOpNode() const;
	bool isConst() const;
	friend class HOptimizer;
	friend class InlineCall;
};

class IntNode : public Node{
public:
	IntNode(Token *t);
	void print(int deep) const;
	void generate(AsmCode &code);
	void print(ofstream *f, int deep) const;
	SymbolType* getType();
	Node* SelfCopy();
	bool isConst() { return true; }
	bool operator == (int val) { return dynamic_cast<IntNumber*>(token)->v2 == val; }
	bool operator == (Node *node) const { return *node == dynamic_cast<IntNumber*>(token)->v2; }
	int operator = (int val) { return dynamic_cast<IntNumber*>(token)->v2 = val; }
	bool isSimple() const {return true; }
};


class FloatNode : public Node{
private:
	string constName() const;

public:
	int index;
	FloatNode(Token *t, int ind);
	void print(int deep) const;
	void print(ofstream* f, int deep) const;
	SymbolType* getType();
	Node* SelfCopy();
	void generate(AsmCode &code);
	void generateLvalue(AsmCode &code);
	void generateFPUStack(AsmCode& code);
	void GenerateData(AsmCode &code) const;
	bool operator == (int val) { return dynamic_cast<FloatNumber*>(token)->v2 == val; }
	bool operator == (Node *node) const { return *node == dynamic_cast<FloatNumber*>(token)->v2; }
	int operator = (int val) { return dynamic_cast<FloatNumber*>(token)->v2 = val; }
	bool isConst() { return true; }
	bool isSimple() const {return true; }
};

class IdentifierNode : public Node{
public:
	VarSymbol *var;
	int assignCounter;
	IdentifierNode(Token *t, VarSymbol *v);
	IdentifierNode(const string & name, int line, int col);
	IdentifierNode(const string & name, VarSymbol *v);
	IdentifierNode(VarSymbol *v);
	SymbolType* getType();
	void print(int deep) const;
	void print(ofstream* f, int deep) const;
	bool isLvalue() const;
	Node* SelfCopy();
	Node* calculate();
	bool operator == (Node *node) const;
	bool isConst() const;
	bool isIdent() const { return true; }
	bool isModifiableLvalue() const;
	void generate(AsmCode &code);
	void generateLvalue(AsmCode &code);
	void generateFPUStack(AsmCode& code);
};

class CharNode : public Node{
public:
	CharNode(Token *t); 
	void print(int deep) const;
	SymbolType* getType();
	bool isConst() { return true; }
	Node* SelfCopy();
	bool operator == (int val) { return dynamic_cast<Char*>(token)->v2 == val; }
	bool operator == (Node *node) const { return *node == dynamic_cast<Char*>(token)->v2; }
	int operator = (int val) { return dynamic_cast<Char*>(token)->v2 == val; }
	bool isSimple() const {return true; }
	void print(ofstream* f, int deep) const;
};

class StringNode : public Node{
public:
	int index;
	StringNode(Token *t, int ind);
	void print(int deep) const;
	SymbolType *getType();
	void GenerateData(AsmCode &code) const;
	bool isConst() { return true; }
	Node* SelfCopy();
	bool isString() const { return true; }
	void print(ofstream* f, int deep) const;
};

class FunctionalNode : public Node{
protected:
	Node *name;
	vector<Node*> args;

	void printArgs(int deep) const;
	void printArgs(ofstream *f, int deep) const;
	void print(int deep) const;
	void print(ofstream *f, int deep) const;

public:
	FunctionalNode(Node *n);
	FunctionalNode(Token *t, Node *n);
	Node* SelfCopy();
	void ReplaceCallNode(FuncCallNode *what, Node *to);
	vector<FuncCallNode*>* GetInlineFunction();
	void ReplaceDiv2Mult();
	void SetIndex(int i);
	void generateFPUStack(AsmCode& code);
	void addArg(Node *arg);
	friend class HOptimizer;
	friend class SubExpressionTap;
	friend class InlineCall;
};

class FuncCallNode : public FunctionalNode{
private:
	FuncSymbol *symbol;

public:
	FuncCallNode(Token* t, Node* func, FuncSymbol* funcsym): FunctionalNode(t, func), symbol(funcsym) {}
	void generate(AsmCode& code);
	SymbolType* getType();
	vector<FuncCallNode*>* GetInlineFunction();
	bool isRecursiveFuncCallNode() const;
	bool isInlineSubstPossibe() const;
	bool isVoidFuncCallNode() const;
	void print(int deep) const;
	friend class FuncSymbol;
	friend class InlineCall;
};

class IONode : public FunctionalNode{
private:
	KeyWord *token;
	StringNode *format;

public:
	friend class Parser;
	IONode(KeyWord *t, StringNode *n) : token(t), format(n), FunctionalNode(0, 0) {};
	SymbolType *getType();
	void generate(AsmCode& code);
	void print(int deep) const;
};

class ArrNode : public FunctionalNode{
public:
	ArrNode(Node* arr);
	void print(int deep) const;
	void print(ofstream *f, int deep) const;
	bool isLvalue() const;
	void generate(AsmCode &code);
	void generateLvalue(AsmCode &code);
	bool isModifiableLvalue() const;
	SymbolType *getType();
};

class Statement : public Node{
	virtual void print(int deep = 0) {}
	bool isExpression() const { return false; }
	virtual void ReplaceDiv2Mult() {}
	friend class Parser;
	friend class Block;
};

class SubExpressionBlock;

class Block : public Statement{
private:
	vector<Node*> body;
	SymTable *table;
	SubExpressionBlock *exprBlock;
	AsmArgLabel *endLabel;
	static unsigned long int blockCounter;
	int blockInserter;
	bool copy;

public:
	friend class Parser;
	Block(const	Block &block, SymTable *table = 0);
	Block() : table(new SymTableForLocals()), exprBlock(0), blockInserter(0), endLabel(0), copy(false) {}
	Block(SymTableForLocals *t) : table(t), exprBlock(0), blockInserter(0), endLabel(0), copy(false) {}			//!!!
	bool isBlock() const { return true; }
	virtual void generate(AsmCode& code);
	void ReplaceDiv2Mult();
	void initSubExprBlock(SubExpressionBlock *e);
	SubExpressionBlock *GetExprBlock() const;
	Node *SelfCopy();
	void AddStatement(Node *st) { body.push_back(st); }
	int size() const;
	void print(int deep = 0) const;
	friend class SubExpressionTap;
	friend class InlineCall;
	friend class SymTable;
	friend class FuncSymbol;
	friend class FuncCallNode;
	friend class GoToNode;
};

class IfStatement : public Statement{
private:
	Node *condition;
	Node *if_branch;
	Node *else_branch;

public:
	static unsigned long labelCounter;
	bool isIfStatement() const { return true; }
	IfStatement(Node *cond, Node *$if, Node *$else = 0) : condition(cond), if_branch($if), else_branch($else) {}
	void generate(AsmCode &code);
	void print(int deep = 0) const;
	Node* SelfCopy();
	friend class Parser;
	friend class SubExpressionTap;
	friend class InlineCall;
};

class CycleStatement : public Statement{
protected:
	AsmArgLabel *startLabel;
	AsmArgLabel *endLabel;


public:
	Node *body;
	friend class Parser;
	friend class ContinueStatement;
	friend class BreakStatement;
	void ReplaceDiv2Mult();
	bool isCycle() const { return true; }

	CycleStatement(Node *b) : body(b), startLabel(0), endLabel(0) {}
};

class ForStatement : public CycleStatement{
private:
	Node *first_cond;
	Node *second_cond;
	Node *third_cond;
	AsmArgLabel *third_cond_label;

public:
	static unsigned long labelCounter;
	friend class ContinueStatement;
	friend class BreakStatement;
	ForStatement(Node *first, Node *second, Node *third, Node *block) 
		: CycleStatement(block), first_cond(first), second_cond(second), third_cond(third) {}
	Node* SelfCopy();
	void generate(AsmCode &code);
	void print(int deep = 0) const;
	friend class Parser;
	friend class InlineCall;
};


class WhileStatement : public CycleStatement{
private:
	Node *condition;

public:
	static unsigned long labelCounter;
	WhileStatement(Node *c, Node *b) : CycleStatement(b), condition(c) {}
	Node* SelfCopy();
	void generate(AsmCode &code);
	void print(int deep = 0) const;
	friend class Parser;
	friend class InlineCall;
};

class DoWhileStatement : public CycleStatement{
private:
	Node *condition;

public:
	static unsigned long labelCounter;
	DoWhileStatement(Node *c, Node *b) : CycleStatement(b), condition(c) {}
	void generate(AsmCode &code);
	Node* SelfCopy();
	void print(int deep = 0) const;
	friend class Parser;
	friend class InlineCall;
};

class JumpStatement : public Statement{
protected:
	CycleStatement *owner;

public:
	JumpStatement(CycleStatement *o) : owner(o) {}
};

class ContinueStatement : public JumpStatement {
public:
	ContinueStatement(CycleStatement *o) : JumpStatement(o) {}
	void generate(AsmCode &code);
	void print(int deep = 0) const;
	Node* SelfCopy();
};

class BreakStatement : public JumpStatement {
public:
	void generate(AsmCode &code);
	BreakStatement(CycleStatement *o) : JumpStatement(o) {}
	void print(int deep = 0) const;
	Node *SelfCopy();
};

class ReturnStatement : public Statement {
private:
	Node *value;
	FuncSymbol *owner;
	friend class Parser;
	friend class SubExpressionTap;
	friend class InlineCall;

public:
	ReturnStatement(Node *v, FuncSymbol *o) : value(v), owner(o) {}
	bool isReturnExpression() const { return true; }
	Node* SelfCopy();
	void generate(AsmCode &code);
	void print(int deep = 0) const;
};


extern string real4name;
extern string real8name;
extern AsmMemory *real4;
extern AsmMemory *real8;

extern int floatCount;
extern vector<FloatNode*> floatConsts;