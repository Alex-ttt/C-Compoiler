#pragma once
#include "parser.h"
#include <string>

Parser::Parser(Scan &l, CodeGenerator *gen) : lexer(l), generator(gen), parsingFunc(0), struct_counter(0), inlineFuncs(false){

	lexer.Next();

	priorityTable[cl_sq_bracket] = priorityTable[op_sq_bracket] = 
	priorityTable[op_bracket] = priorityTable[cl_bracket] = priorityTable[point] = priorityTable[arrow] = 15;

	priorityTable[decr] = priorityTable[incr] =priorityTable[b_not] = priorityTable[l_not] = 14;

	priorityTable[percent] = priorityTable[divis] = priorityTable[factor] = 13;

	priorityTable[Values::minus] = priorityTable[Values::plus] = 12;

	priorityTable[sr] = priorityTable[sl] = 11;

	priorityTable[less_or_eql] = priorityTable[more_or_eql] =
	priorityTable[more] = priorityTable[Values::less] = 10;

	priorityTable[not_eql] = priorityTable[equally] = 9;

	priorityTable[b_and] = 8;
	
	priorityTable[xor] = 7;

	priorityTable[b_or] = 6;

	priorityTable[l_and] = 5;

	priorityTable[l_or] = 4;

	priorityTable[quest] = 3;
	priorityTable[colon] = 0; 

	priorityTable[xor_eql] = priorityTable[or_eql] = priorityTable[and_eql] =
	priorityTable[per_eql] = priorityTable[div_eql] = priorityTable[fct_eql] =
	priorityTable[mn_eql]  = priorityTable[pl_eql] = priorityTable[assign] = 2;

	priorityTable[comma] = 1;

	right[b_and] = right[b_not] = right[l_not] = right[xor_eql] = 
	right[or_eql] = right[and_eql] = right[per_eql] = right[div_eql] = 
	right[fct_eql] = right[mn_eql] = right[pl_eql] = right[assign] = true;

	unary[l_not] = unary[b_not] = unary[factor] = unary[decr] = unary[incr] =
	unary[b_and] = unary[plus] = unary[minus] = true;

	typePriority[CharType] = 1;
	typePriority[IntType] = 2;
	typePriority[FloatType] = 3;

	operationTypeOperands[percent] = IntType;
	operationTypeOperands[b_and] = IntType;
	operationTypeOperands[b_or] = IntType;
	operationTypeOperands[xor] = IntType;
	operationTypeOperands[b_not] = IntType;
	operationTypeOperands[sl] = IntType;
	operationTypeOperands[sr] = IntType;

	operationReturningType[percent] = IntType;
	operationReturningType[b_and] = IntType;
	operationReturningType[b_or] = IntType;
	operationReturningType[b_not] = IntType;
	operationReturningType[sl] = IntType;
	operationReturningType[sr] = IntType;
	operationReturningType[l_and] = IntType;
	operationReturningType[l_or] = IntType;
	operationReturningType[xor] = IntType;
	operationReturningType[l_not] = IntType;
	operationReturningType[equally] = IntType;
	operationReturningType[not_eql] = IntType;
	operationReturningType[Values::less] = IntType;
	operationReturningType[less_or_eql] = IntType;
	operationReturningType[more] = IntType;
	operationReturningType[more_or_eql] = IntType;

	SymTable* basic_def = new SymTable();
	basic_def->add_symbol(IntType);
	basic_def->add_symbol(FloatType);
	basic_def->add_symbol(CharType);
	basic_def->add_symbol(VoidType);
	
	global_field.table = basic_def;
	tablesStack.push(basic_def);	
}

bool Parser::ContainReturnStatement(Block *block){
	for(Node *expr : block->body){
		if(expr->isReturnExpression())
			return true;
		if(Block *b = dynamic_cast<Block*>(expr))
			if(ContainReturnStatement(b))
				return true;
		if(IfStatement *_if = dynamic_cast<IfStatement*>(expr))
			if(ContainReturnStatement(dynamic_cast<Block*>(_if->if_branch)) ||
				ContainReturnStatement(dynamic_cast<Block*>(_if->else_branch)))
				return true;
		if(CycleStatement *loop = dynamic_cast<CycleStatement*>(expr))
			if(ContainReturnStatement(dynamic_cast<Block*>(loop->body)))
				return true;
	}
}

vector<FloatNode*> Parser::floatConsts;

void Parser::TurnOnInline(){
	inlineFuncs = true;
}

void Parser:: throwexception(bool condition, char *massage, Token *token){
	if(condition)
		throw MyException(massage, token);
}

void Parser:: throwexception(bool condition, string massage, Token *token){
	if(condition)
		throw MyException(massage, token);
}

void Parser::throwexception(bool condition, char *massage, int line, int col){
	if(condition)
			throw MyException(massage, line, col);	
}

FunctionalNode* Parser::ParseFuncCall(Node *r){
	Token* next = lexer.Get();
	FuncCallNode* f = new FuncCallNode(r->token, r, dynamic_cast<FuncSymbol*>(r->getType()));
	lexer.Next();
	Token* t = lexer.Get();
	while(*t != cl_bracket){
		Node* s = ParseExpression(priorityTable[comma] + 1);
		f->addArg(s);
		t = lexer.Get();
		throwexception(lexer.isEnd(), "Expected closing bracket after function argument list", lexer.Get()->line, lexer.Get()->start + t->str.length());
		if(*t == comma){
			lexer.Next();
			t = lexer.Get();
		}
	}
	lexer.Next();
	return f;
}

void Parser::print_declaration(int deep) const {
	tablesStack.print(deep);
}

void Parser::print() const {
	if(global_field.body.size())
		cout << ' ' << "Statements:" << endl;
	for(int i = 0; i < global_field.body.size(); i++)
		global_field.body[i]->print(1);
	
	if(global_field.table->sym_ptr.size()){
		cout << ' ' << "Symbols Table:" << endl;
		for(int i = 0; i < top.size(); i++){
			cout << ' ';
			top.sym_ptr[i]->print(1);	
		}	
	}
	if(!global_field.table->sym_ptr.size() && !global_field.body.size())
		cout << ' ' << "<< block is empty >>" << endl;
}

ArrNode* Parser::ParseArrIndex(Node *root){
	Token *next = lexer.Get();
	ArrNode *r = new ArrNode(root);
	Token *t = lexer.Get();
	while(*t == op_sq_bracket){
		lexer.Next();
		Node *index = ParseExpression();
		dynamic_cast<ArrNode*>(r)->addArg(index);
		t = lexer.Get();
		throwexception(*t != cl_sq_bracket, "Expected bracket close after array index", lexer.Get()->line, lexer.Get()->start + t->str.length());
		lexer.Next();
		t = lexer.Get();
	}
	return r;
}

Node* Parser::ParseFactor(){
	Node *root = 0;
	Token *token = lexer.Get();
	if(*token == semicolon)
		return new EmptyNode();
	switch (token->type){
	case T_INTEGER:
		root = new IntNode(token);
		break;

	case T_FLOAT:
		root = new FloatNode(token, floatConsts.size());
		floatConsts.push_back(dynamic_cast<FloatNode*>(root));
		floatCount++;
		break;
		
	case T_IDENTIFIER:
	{	
		Symbol *sym = tablesStack.find_symbol(token->str);
		if(!sym && parsingFunc)
			sym = parsingFunc->params->find_symbol(token->str);
		string exc = "Identifier \"" + token->str + "\" not defined";
		throwexception(!sym, exc.c_str(), token);
		throwexception(!dynamic_cast<VarSymbol*>(sym) && !dynamic_cast<FuncSymbol*>(sym), "Unknown symbol", token);
		throwexception(!sym, "Identifier is undefined", token);
		root = new IdentifierNode(token, dynamic_cast<VarSymbol*>(sym));
		if(*GetNext() == op_brace)
			root = ParseFuncCall(root);
		return root;
	}

	case T_CHAR:
		root = new CharNode(token);
		break;

	case T_STRING:
		root = new StringNode(token, stringConsts.size());
		stringConsts.push_back(dynamic_cast<StringNode*>(root));
		break;

	case T_KEYWORD:
		{
			Values kw = token->value;
			if(kw == _printf || kw == _scanf){
				throwexception(*GetNext() != op_bracket, "Open bracket expected", token);
				lexer.Next();
				StringNode *format = dynamic_cast<StringNode*>(ParseExpression(priorityTable[comma] + 1));
				throwexception(!format, "Expected string format", token);
				IONode *node = new IONode(dynamic_cast<KeyWord*>(token), format);
				if(*lexer.Get() == comma){
					lexer.Next();
					while(true){
						Node *arg = ParseExpression(priorityTable[comma] + 1);
						throwexception(!arg, "Argument expected", token); 
						node->addArg(arg);
						if(*lexer.Get() == cl_bracket)
							break;
						if(*lexer.Get() == comma)
							lexer.Next();
					}
				}
				lexer.Next();
				root = node;
			} else if(kw == _char || kw == _int || kw == _float){
				lexer.Next();
				Token *t = lexer.Get();
				string typeName = kw == _char ? "char" : kw == _int ? "int" : "float";
				throwexception(*lexer.Get() != op_bracket, "Expected open bracket '('", token);
				root = new CastNode(token, ParseExpression(), dynamic_cast<SymbolType*>(tablesStack.find_symbol(typeName)));
				//throwexception((*lexer.Get() != cl_bracket && !lexer.isEnd()), "Expected closing bracket", token);
			}
			else 
				throw MyException("You can use keywords just such as char, int and float in expressions", token);
		}
		break;

	case T_OPERATION:
			if(unary[dynamic_cast<Operation*>(token)->value]){
				lexer.Next();
				root = new UnaryOpNode(token, ParseExpression(priorityTable[decr]));
			} else 	if(*token == op_bracket){
				lexer.Next();
				root = ParseExpression();
				if(*lexer.Get() != cl_bracket)
					throw MyException("Expected closing bracket", token);
			}
			else 
				throw MyException("Wrong expression", token);	
		break;
	}
	if (!(token->type == T_OPERATION && unary[token->value]) && *token != _printf && *token != _scanf)
		lexer.Next();
	return root;
}

Node* Parser::ParseMember(Node* left){
	SymbolType *type = left->getType();
	StructSymbol *struct_type = 0;
	if(dynamic_cast<PointerSymbol*>(type))
		struct_type = dynamic_cast<StructSymbol*>(dynamic_cast<PointerSymbol*>(type)->pointer);
	else
		struct_type = dynamic_cast<StructSymbol*>(type);
	throwexception(!struct_type, "Left operand of . or -> must be a structure", lexer.Get());
	Token* opTok = lexer.Get();
	Token* token = GetNext();
	throwexception(!dynamic_cast<Identifier*>(token), "Right operand of . or -> must be a identifier", token);
	string fieldName = token->str;
	if (!struct_type->s_table->exists(fieldName))
		fieldName = '%' + fieldName;
	throwexception(!struct_type->s_table->exists(fieldName), "Undefined field in structure");
	lexer.Next();
	Node* right = new IdentifierNode(token, dynamic_cast<VarSymbol*>(struct_type->s_table->find_symbol(fieldName)));
	return new BinaryOpNode(opTok, left, right);
}

Node* Parser::ParseExpression(int priority){
	if (priority > 15)
		return ParseFactor();
	Node *left = ParseExpression(priority + 1);
	Node *root = left;
	Token *op = lexer.Get();
	if(lexer.isEnd() || *op == cl_brace || *op == cl_bracket  || *op == cl_sq_bracket ||
		*op == colon || *op == semicolon || *op == op_brace || *op == cl_brace){			
			root->getType();
		return root;
	}
	throwexception(op->type != T_OPERATION && (*op != comma), "Invalid expression. Operation expected", op->line, op->start);
	if(priorityTable[op->value] < priority)
		return root;
	while(!lexer.isEnd() && dynamic_cast<Operation*>(op) && (priorityTable[dynamic_cast<Operation*>(op)->value] >= priority) && op->value != cl_bracket){
		Values oper = dynamic_cast<Operation*>(op)->value;
		if(oper == op_bracket){
			root = ParseFuncCall(root);
		}
		else if(oper == op_sq_bracket){
			root = ParseArrIndex(root);
			break;
		}
		else if(oper == incr ||oper == decr){
			root = new PostfixUnaryOpNode(op, root);
			lexer.Next();	
		}
		else if(oper == quest){
			lexer.Next();
			Node* l = ParseExpression();
			if(lexer.Get()->value != colon)
				throw MyException("Invalid ternary operator", lexer.Get());
			lexer.Next();
			Node* r = ParseExpression();
			root = new TernaryOpNode(op, root, l, r);
		} else if(oper == point || oper == arrow) 
			root = ParseMember(root);		
		else
		{
			if(*op == cl_sq_bracket)
				break;
			lexer.Next();
			root = new BinaryOpNode(op, root, ParseExpression(priority + !right[oper]));	
		}
		op = lexer.Get();
	}
	root->getType();
	return root;
}

Token* Parser ::GetNext(){
	lexer.Next();
	return lexer.Get();
}

StructSymbol* Parser::ParseStruct(bool param){
	Token *token = GetNext();
	string name;
	if(*token == val_ident){
		name = token->str;
		token = GetNext();
	} else 
		name = "unnamed struct " + to_string((long double)struct_counter++);
	
	StructSymbol *struct_symbol = dynamic_cast<StructSymbol*>(tablesStack.find_symbol(name));
	throwexception(param && !struct_symbol, "Unknown struct type"); 

	if(param && !struct_symbol){
		throwexception(*token == op_brace, "Using the definition type are not allowed", token);
		throw MyException("Expected type's name", token);
	}

	if(!struct_symbol){
		struct_symbol = new StructSymbol(0, name);
		if(name.length() > 0)
			tablesStack.add_symbol(struct_symbol);	
	}
	
	if(*token == op_brace){
		throwexception(param, "Using the definition type are not allowed", token);
		throwexception(struct_symbol->s_table, "Struct redefinetion", token);
		struct_symbol->s_table = new SymTableForFields();
		tablesStack.push(struct_symbol->s_table);
		token = GetNext();
		while(*token != cl_brace){
			SymbolType *type = ParseType();
			lexer.Next();
			while(*lexer.Get() != semicolon){
				struct_symbol->s_table->add_symbol(ParseIdentifier(type));
				token = lexer.Get();
				if(*token == comma)
					token = GetNext();				
			}
			token = GetNext();
		}
		tablesStack.pop();
	}
	return struct_symbol;
}


SymbolType * Parser::ParseType(bool param){

	Token *token = lexer.Get();
	SymbolType *type = 0;
	bool Const = false, Inline= false;

	if(*token == _const){
		Const = true;
		token = GetNext();
	}

	if(*token == _Inline){
		Inline = true;
		token = GetNext();
	}
	if(*token == _struct)
		type = ParseStruct(param);
	else {
		type = dynamic_cast<SymbolType*>(tablesStack.find_type(token->str));
		throwexception(!type, "Unknown type", token);	
	}
	if(Const)
		type = new ConstSymbolType(type);
	type->SetInline(Inline);
	return type;
}

SymbolType* Parser::ParseArrayDimension(SymbolType *type, bool param){			
	vector <int> index;
	Token *token = lexer.Get();
	
	while(*token == op_sq_bracket){
		token = GetNext();
		int size = *token == val_int ? dynamic_cast<IntNumber*>(token)->v2 : 0;
		throwexception(size <= 0, "Array size must be int number higher than zero", token);
		throwexception(size >= 1e5, "Array size exceeds the allowable", token);
		index.push_back(size);
		token = GetNext();	
		throwexception(*token != cl_sq_bracket, "Closing bracket expected  after the array index", token);
		token = GetNext();
	}

	for(int i = index.size() - 1; i >= 0; i--)
		type = new ArraySymbol(type, index[i]);
	return type;
}

void Parser::ParseParam(){
	SymbolType *type = ParseType(true);
	Token *token = GetNext();
	throwexception(type->name == "void" && *token != factor, "Parameter can not be of type void", token);
	while(*token == factor){
		type = new PointerSymbol(type);
		token = GetNext();
	}
	if(*token == op_bracket)
		type = dynamic_cast<SymbolType*>(ParseComplexDeclaration(type));
	string name = *token == val_ident ? token->str : "";
	if(name.length() > 0){
		throwexception(tablesStack.top()->isExist(name), "Redefinition", token);
		token = GetNext();
	}

	if(*token == op_sq_bracket)
		type = ParseArrayDimension(type);
	VarSymbol *s = new VarSymbol(name, type);
	s->global = false;
	tablesStack.add_symbol(s);
}

void Parser::ParseArgList(){
	Token *token = GetNext(); 
	while(*token != cl_bracket){
		ParseParam();
		token = lexer.Get();
		if(*token == comma)
			token = GetNext();
		else 
			throwexception(*token != cl_bracket, "Closing bracket expected", token);
	}
	lexer.Next();
}

FuncSymbol* Parser::createFunction(const string &name, SymbolType *type){
	FuncSymbol *func = new FuncSymbol(name, type);
	func->params = new SymTableForParams();
	tablesStack.push(func->params);
	ParseArgList();
	tablesStack.pop();
	return func;	
}

VarSymbol * Parser::ParseIdentifier(SymbolType *type, bool param){

	VarSymbol *result = 0;
	Token *token = lexer.Get();

	while(*token == factor){
		type = new PointerSymbol(type);
		token = GetNext();
	}

	if(*token == op_bracket)
		return ParseComplexDeclaration(type);

	throwexception(*token != val_ident, "Identifier expected", token);
	string name = token->str;
	token = GetNext();

	if(*token != op_bracket){
		throwexception(type->name == "void", "Incomplete type is invalid", token);
		if(*token == op_sq_bracket)
			type = ParseArrayDimension(type);	
	} else 
		type = createFunction(name, type);

	result = new VarSymbol(name, type);
	token = lexer.Get();
	throwexception(tablesStack.tables.back()->find_symbol(name) != 0, "Redefinition", token);
	throwexception(!(*token == comma || *token == semicolon || *token == assign || *token == op_brace),
		"Semicolon expected", lexer.Get());
	return result;
}

VarSymbol* Parser::ParseDirectDeclaration(){
	VarSymbol *var_sym = 0;
	SymbolType *type = 0;
	while(*GetNext() == factor)
		type = new PointerSymbol(type);
	if(*lexer.Get() == op_bracket)
		var_sym = ParseDirectDeclaration();
	else {
		throwexception(*lexer.Get() != val_ident, "Identifier expected", lexer.Get());
		throwexception(tablesStack.find_symbol(lexer.Get()->str), "Redefinition", lexer.Get());
		var_sym = new VarSymbol(lexer.Get()->str, 0);
		lexer.Next();
	}
		if(*lexer.Get() == op_bracket)
			type = createFunction("", type);
		else if(*lexer.Get() == op_sq_bracket)
			type = ParseArrayDimension(type);

	if(!var_sym->type)
		var_sym->type = type;
	else {
		while(var_sym->type->upType())
			var_sym->type = var_sym->type->upType();
		var_sym->type->initType(type);
	}
	if(*lexer.Get() == cl_bracket)
		lexer.Next();
	return var_sym;
}

VarSymbol* Parser::ParseComplexDeclaration(SymbolType* start_type){
	VarSymbol *var_sym = ParseDirectDeclaration();
	if(*lexer.Get() == op_bracket)
		start_type = createFunction("", start_type);
	else if(*lexer.Get() == op_sq_bracket)
		start_type = ParseArrayDimension(start_type);
//	if(*lexer.Get() == cl_bracket)
//		lexer.Next();
	if(!var_sym->type)
		var_sym->type = start_type;
	else {
		while(var_sym->type->upType())
			var_sym->type = var_sym->type->upType();
		var_sym->type->initType(start_type);
	}
	return var_sym;
}

void Push_Float(FloatNode* node){
	Parser::floatConsts.push_back(node);
}

void Parser::ParseDeclaration(){
	SymbolType *t_symbol = ParseType();
	VarSymbol *var = 0;
	while(true){
		Token *token = GetNext();
		if(*token == semicolon && t_symbol->isStruct())
			break;

		var = *lexer.Get() == op_bracket ? ParseComplexDeclaration(t_symbol) : ParseIdentifier(t_symbol);
		throwexception(t_symbol->GetInline() && !var->isFuncVar(), "Wrong inline using", token);
		tablesStack.add_symbol(var);
		
		if(*lexer.Get() == assign){
			Token *asgn = lexer.Get();
			lexer.Next();
			Node *assign_operand = ParseExpression(priorityTable[comma] + 1);
			IdentifierNode *id = new IdentifierNode(token, var);
			var->init= assign_operand;
			id->getType();
			assign_operand->getType();
			blocks.top()->AddStatement(new BinaryOpNode(asgn, id, assign_operand));
		}

		if(*lexer.Get() == semicolon || *lexer.Get() ==	op_brace)
			break;
		if(*lexer.Get() != comma)
			throw MyException("Comma expected", lexer.Get());
		//lexer.Next();
	}

	if(*lexer.Get() == op_brace){
		FuncSymbol *func = dynamic_cast<FuncSymbol*>(var->type);
		throwexception(!func, "Unexpected brace", lexer.Get());
		throwexception((tablesStack.tables.size() != 1), "Can not define the function in the block", lexer.Get());
		parsingFunc = func;
		func->body = ParseBlock(true);
		parsingFunc = 0;
		throwexception(func->getType() != VoidType && !ContainReturnStatement(func->body),
			"Return statement not found\nFunction: " + func->name);
		func->endLabel = makeLabel("f_" + var->name + "_end");
		lexer.Next();
	} else
		lexer.Next();
}

Block* Parser::ParseBlock(bool is_func){
	SymTable *top = tablesStack.top();
	Block *block = new Block(new SymTableForLocals(is_func ? 4 : top->shift + top->byteSize()));
	tablesStack.push(block->table);
	blocks.push(block);
	if(*lexer.Get() == op_brace){
		Token *token = GetNext();
		while(*token != cl_brace){
			if((*token == _const || *token == _struct || tablesStack.find_type(token->str))
				&&!dynamic_cast<FuncSymbol*>(tablesStack.find_type(token->str)))
			{
				ParseDeclaration();
			}
			else {
				Node *stmt = ParseStatement();
				block->AddStatement(stmt);
				if(!stmt->isIfStatement())
					lexer.Next();
			}
			token = lexer.Get();
		}
	}
	else
	{
		Token *token = lexer.Get();
		if(*token == _const || *token == _struct || tablesStack.find_type(token->str))
				ParseDeclaration();
		else {
			block->AddStatement(ParseStatement());
			//lexer.Next();
		}
	}
	block->endLabel = new AsmArgLabel(CreateUnicNameForBlock());
	blocks.pop();
	tablesStack.pop();
	return block;
}

string Parser::CreateUnicNameForBlock(){
	static string block_name("BLOCK_LABEL");
	return block_name + to_string(Block::blockCounter++);
}

ForStatement* Parser::ParseFor(){
	throwexception(tablesStack.size() < 2, "Can not use \"for\" at the global field", lexer.Get());
	Node *first = 0, *second = 0, *third = 0;
	throwexception((*GetNext() != op_bracket), "Opening bracket expected", lexer.Get());
	lexer.Next();
	first = ParseExpression();
	throwexception((*lexer.Get() != semicolon), "Semicolon expected", lexer.Get());
	lexer.Next();
	second = ParseExpression();
	throwexception((*lexer.Get() != semicolon), "Semicolon expected", lexer.Get());
	lexer.Next();
	if(*lexer.Get() != cl_bracket)
		third = ParseExpression();
	else
		third = new EmptyNode;
	throwexception((*lexer.Get() != cl_bracket), "Closing bracket expected", lexer.Get());
	lexer.Next();
	CycleStatement *tmp = parsingCycle;
	ForStatement *ret = new ForStatement(first, second, third, 0);
	parsingCycle = ret;
	Block *b = ParseBlock();
	parsingCycle = tmp;
	ret->body = b;
	//lexer.Next();
	return ret;
}

IfStatement *Parser::ParseIf(){
	throwexception(tablesStack.size() < 2, "Can not use \"if\" at the global field", lexer.Get());
	throwexception((*GetNext() != op_bracket), "Opening bracket expected", lexer.Get());
	lexer.Next();
	Node *condition = ParseExpression();
	throwexception((*lexer.Get() != cl_bracket), "Closing bracket expected", lexer.Get());
	lexer.Next();
	Node *if_branch = ParseBlock();
	Node *else_branch = 0;
	lexer.Next();

	if(*lexer.Get() == _else){
		lexer.Next();
		else_branch = ParseBlock();
		lexer.Next();
	}
	return new IfStatement(condition, if_branch, else_branch);
}

WhileStatement* Parser::ParseWhile(){
	throwexception(tablesStack.size() < 2, "Can not use \"while\" at the global field", lexer.Get());
	throwexception(*GetNext() != op_bracket, "Opening bracket expected", lexer.Get());
	lexer.Next();
	Node *condition = ParseExpression();
	throwexception(*lexer.Get() != cl_bracket, "Closing bracket expected", lexer.Get());
	lexer.Next();
	WhileStatement *ret = new WhileStatement(condition, 0);
	CycleStatement *tmp = parsingCycle;
	parsingCycle = ret;
	Node *b = ParseBlock();
	ret->body = b;
	parsingCycle = tmp;
	//lexer.Next();
	return ret;
}

DoWhileStatement* Parser::ParseDoWhile(){
	throwexception(tablesStack.size() < 2, "Can not use \"do-while\" at the global field", lexer.Get());
	DoWhileStatement *ret = new DoWhileStatement(0, 0);
	CycleStatement *tmp = parsingCycle;
	lexer.Next();
	parsingCycle = ret;
	Block *body = ParseBlock();
	ret->body = body;
	parsingCycle = tmp;
	throwexception(*GetNext() != _while, "While expected", lexer.Get());
	throwexception(*GetNext() != op_bracket, "Opening bracket expected", lexer.Get());
	lexer.Next();
	ret->condition = ParseExpression();
	throwexception(*lexer.Get() != cl_bracket, "Closing bracket expected", lexer.Get());
	throwexception(*GetNext() != semicolon, "Semicolon expected", lexer.Get());
	//lexer.Next();
	return ret;
}

Node* Parser::ParseStatement(){
	switch(lexer.Get()->value){
	case _if:
		return ParseIf();
	case _for:
		return ParseFor();
	case _while:
		return ParseWhile();
	case _do:
		return ParseDoWhile();
	case op_brace:
		return ParseBlock();
	case _return:
	case _break:
	case _continue:
		return ParseJumpStatement();
	default:
		return ParseExpression();
	}
}

Statement* Parser::ParseJumpStatement(){
	Statement *jump = 0;
	switch(lexer.Get()->value){
		case _return:
		{
			throwexception(!parsingFunc, "Unexpected return statement", lexer.Get());
			Node *arg = *GetNext() != semicolon ? ParseExpression() : 0;
			jump = new ReturnStatement(arg, parsingFunc);
			break;
		}
		case _break:
			throwexception(!parsingCycle, "Unexpected break statement", lexer.Get());
			jump = new BreakStatement(parsingCycle);
			lexer.Next();
			break;
		case _continue:
			throwexception(!parsingCycle, "Unexpected continue statement", lexer.Get());
			jump = new ContinueStatement(parsingCycle);
			lexer.Next();
			break;
	}
	throwexception(*lexer.Get() != semicolon, "Semicolon expected", lexer.Get());
	return jump;
}

void Parser::CreateTopTable(){
	for(int i = 4; i < global_field.table->size(); i++)
		top.add_symbol(global_field.table->sym_ptr[i]);
}

void Parser::GenerateCode(){
	throwexception(!generator, "You need to create asm code generator");

	for(int i = 0; i < stringConsts.size(); i++)
		stringConsts[i]->GenerateData(generator->data);
	for(int i = 0; i < floatConsts.size(); i++)
		floatConsts[i]->GenerateData(generator->data);
	top.generateGlobals(generator->data);
	generator->data.add(cmdREAL4, makeArgMemory("tmp4"), makeFloat(0))
		.add(cmdREAL8, makeArgMemory("tmp8"), makeFloat(0));
	top.generateCode(generator->code);
	generator->code.add(makeLabel("start"))
		.add(cmdCALL, makeLabel("f_main"))
		.add(cmdRET, makeArg(0));
}

void Parser::fflush(){
	generator->generate();
}

Node* Parser::NodeOptimization(Node *node){
	HOptimizer opt(node);
			do{
				opt.constFolding(node);
				node = opt.result();
			} while(!opt.OptimizationComplete());
	return node;
}

void Parser::BlockOptimization(Block *block){
	if(!block  || block->body.size() == 0 && block->table->sym_ptr.size() == 0)
		return;
	SymTable *table = block->table;
	for(int i = 0; i < table->sym_ptr.size(); i++)
		if(VarSymbol *var = dynamic_cast<VarSymbol*>(table->sym_ptr[i])){
			if(FuncSymbol *func = dynamic_cast<FuncSymbol*>(var->type))
				BlockOptimization(func->body);				
			else 
				var->init = NodeOptimization(var->init);
		}

	for(int i = 0; i < block->size(); i++){
		Node *cur_node = block->body[i];
		if(Statement *st = dynamic_cast<Statement*>(cur_node)){
			if(st->isBlock())
				BlockOptimization(dynamic_cast<Block*>(st));
			else{
				if(IfStatement *$if = dynamic_cast<IfStatement*>(st)){
					$if->condition = NodeOptimization($if->condition);
					if($if->condition->isSimple()){
						if(*$if->condition == 0){
							if($if->else_branch){
								delete $if->if_branch;
								Block *bl = dynamic_cast<Block*>($if->else_branch);
								delete block->body[i];
								BlockOptimization(bl);
								block->body[i] = bl;
							} else {
								delete block->body[i];
								block->body.erase(block->body.begin() + i);
							}
						} else {
							if($if->else_branch){
								delete $if->else_branch;
								$if->else_branch = 0;
							}
							Block *bl = dynamic_cast<Block*>($if->if_branch);
							delete block->body[i];
							BlockOptimization(bl);
							block->body[i] = bl;
						}

					} else {
						BlockOptimization(dynamic_cast<Block*>($if->if_branch));
						if($if->else_branch)
							BlockOptimization(dynamic_cast<Block*>($if->else_branch));
					}

				} else
				if(ForStatement *$for = dynamic_cast<ForStatement*>(st)){
					$for->second_cond = NodeOptimization($for->second_cond);
					if($for->second_cond == 0){
						delete $for->first_cond;
						delete $for->second_cond;
						delete $for->third_cond;
						delete block->body[i];
						block->body.erase(block->body.begin() + i);
					} else {
						$for->first_cond = NodeOptimization($for->first_cond);
						$for->third_cond = NodeOptimization($for->third_cond);
						BlockOptimization(dynamic_cast<Block*>($for->body));
					}
				} else
				if(WhileStatement *$while = dynamic_cast<WhileStatement*>(st)){
					$while->condition = NodeOptimization($while->condition);
					bool ok = $while->condition == 0;
					if(*$while->condition == 0){
						delete $while->body;
						delete block->body[i];
						block->body.erase(block->body.begin() + i);
					} else
						BlockOptimization(dynamic_cast<Block*>($while->body));
				} else
				if(DoWhileStatement *$dowhile = dynamic_cast<DoWhileStatement*>(st)){
					$dowhile->condition = NodeOptimization($dowhile->condition);
					if(*$dowhile->condition == 0){
						Block *bl = dynamic_cast<Block*>($dowhile->body);
						delete block->body[i];
						block->body[i] = bl;
					} else
						BlockOptimization(dynamic_cast<Block*>($dowhile->body));
				} else
				if(ReturnStatement *$return = dynamic_cast<ReturnStatement*>(st)){
					if($return->value)
						$return->value = NodeOptimization($return->value);
					for(int k = block->size() - 1; k > i ; k--){
						delete block->body[k];
						block->body.pop_back();
					}
				}
			}
		}
		else 
			block->body[i] = NodeOptimization(cur_node);
	}
}

void Parser::HighLevelOptimization(){
	BlockOptimization(&global_field);
}

void Parser::SubExpressionReduce(){
	SubExpressionTap tap;
	for(int i = 0; i < top.size(); i++){
		if(!top.sym_ptr[i]->isFuncVar())
			continue;
		Block *block = (dynamic_cast<FuncSymbol*>(dynamic_cast<VarSymbol*>(top.sym_ptr[i])->type))->body;
		tap.FindCommonExpr(block);
		tap.Optimize(block);
	}
	tap.FindCommonExpr(&global_field);
	tap.Optimize(&global_field);
}

void Parser::LowLevelOptimization(){
	Optimizer opt;
	opt.optimize(generator->code);
}

void Parser::ParseProgram(){
	while (!lexer.isEnd()){
		if (*lexer.Get() == _const || *lexer.Get() == _struct || *lexer.Get() == _Inline ||
			dynamic_cast<SymbolType*>(tablesStack.find_symbol(lexer.Get()->str)))
			ParseDeclaration();
		else 
			global_field.body.push_back(ParseExpression());
	}
	VarSymbol *main = dynamic_cast<VarSymbol*>(global_field.table->sym_ptr[global_field.table->index["main"]]);
	throwexception(!main || !main->type->isFunc(), "Can't find main function");
	CreateTopTable();
	if(inlineFuncs){
		InlineCall inlineHandler;
		inlineHandler.ExecInline(dynamic_cast<FuncSymbol*>(main->type)->body);
	}
}
