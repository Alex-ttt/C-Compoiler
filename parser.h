#pragma once

#include "nodes.h"
#include "l_optimizer.h"
#include "h_otimizer.h"
#include <stack>

class Parser{
private:
	Scan &lexer;	
	int struct_counter;
	map <Values, int> priorityTable;
	map <Values, bool> right;
	map <Values, bool> unary;
	SymTableStack tablesStack;
	Block global_field;
	stack<Block*> blocks;
	FuncSymbol *parsingFunc;
	CycleStatement *parsingCycle;
	CodeGenerator *generator;
	SymTable top;
	bool inlineFuncs;


	Node* ParseFactor();	
	FunctionalNode *ParseFuncCall(Node *r);
	ArrNode* ParseArrIndex(Node *r);
	Node* ParseMember(Node* left);
	Token* GetNext();
	void SymCreate(SymTable *table, const string& name, S_types tp = t_Int );
	void ParseArgList();
	void ParseParam();
	SymbolType *ParseType(bool param = false);
	VarSymbol *ParseIdentifier(SymbolType *type, bool param = false);
	SymbolType *ParseArrayDimension(SymbolType *type, bool param = false);
	FuncSymbol* createFunction(const string &name, SymbolType *type);
	StructSymbol *ParseStruct(bool param = false);
	VarSymbol* ParseComplexDeclaration(SymbolType* type);
	VarSymbol* ParseDirectDeclaration();
	void CreateTopTable();
	
	void BlockOptimization(Block *block);
	Node* NodeOptimization(Node *node);

	ForStatement* ParseFor();
	IfStatement* ParseIf();
	WhileStatement* ParseWhile();
	DoWhileStatement* ParseDoWhile();
	Block *ParseBlock(bool is_func = false);
	Statement *ParseJumpStatement();
	static string CreateUnicNameForBlock();

	void throwexception(bool condition, char *massage, Token *token);
	void throwexception(bool condition, string massage, Token *token = 0);
	void throwexception(bool condition, char *massage, int line = -1, int col = -1);

public:
	vector<StringNode*> stringConsts;
	static vector<FloatNode*> floatConsts;
	Node *ParseStatement();
	void ParseProgram();
	void HighLevelOptimization();
	void SubExpressionReduce();
	void LowLevelOptimization();
	void print_declaration(int deep = 0) const;
	void GenerateCode();
	void fflush();
	bool ContainReturnStatement(Block *block);
	void TurnOnInline();
	void print() const;
	Parser(Scan& scaner, CodeGenerator *gen = 0);
	void ParseDeclaration();
	Node* ParseExpression(int priority = 0);
};

