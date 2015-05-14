#pragma once
#include "nodes.h"
#include <map>
#include <stack>

enum Place{
	Root,
	BinLeft,
	BinRight,
	UnOperand,
	FuncArg,
	BaseOfTern,
	Ident
};

class HOptimizer{
private:
	Node *tree;
	Node *current;
	bool optimized;
	Place place;
	int argIndex;

public:
	HOptimizer(Node *_tree);
	bool OptimizationComplete();
	Node* result();
	void constFolding(Node *node, Place p = Root);
	void replaceNode(Node *_new, Node *_replaced, Place p);	
};

class SubExpressionBlock{
private:
	map<Values, vector<Node*>*> expressions;

public:
	friend class SubExpressionTap;
	void Add(Values value, Node *node);
	void Add(Node *node);
};

class SubExpressionTap{
private:
	stack<Block*> blocks;
	unsigned long int temp_counter;

public:
	SubExpressionTap();
	void FindCommonExpr(Block *block);
	void Optimize(Block *block);
	Node* OptimizeExpression(Node *node);
	bool IsChangedValue(Values v) const;
	void CreateNewIdent(Node *node);
	void ReplaceFirstOccur();
	Node* ReplaceFirstOccurInNode(Node *node);
	bool GetArgs(SubExpressionBlock &expBlock, Node *node);
	bool IsModified(Node *node) const;
	bool isTepmInit(Node *node) const;
};

class InlineCall{
private:
	stack<Block*> blocks;
	int insertIndex;
	int blockShift;
	int inlineCounter;
	int ArgInCallIndex(SymTable *table, IdentifierNode *iNode);
	void HandleCallNode(Node *&expr);
	void HandleInlineNodeOccur(Node *&expr, SymTable *formalParams, VarSymbol *returnVar);
	void HandleBlock(Block *block, SymTable *formalParams, VarSymbol *returnVar, AsmArgLabel *label);


public:
	InlineCall();
	void ExecInline(Block *block);
};