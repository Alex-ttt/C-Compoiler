#include "nodes.h"
#include <fstream>

ScalarSymbol *IntType = new ScalarSymbol("int", t_Int);
ScalarSymbol *FloatType = new ScalarSymbol("float", t_Float);
ScalarSymbol * CharType = new ScalarSymbol("char", t_Char);
ScalarSymbol *VoidType = new ScalarSymbol("void", t_Void);
PointerSymbol *StringType = new PointerSymbol(CharType);

string real4name("tmp4");
string real8name("tmp8");
AsmMemory* real4 = new AsmMemory(real4name, false);
AsmMemory* real8 = new AsmMemory(real8name, false);

int floatCount = 0;

unsigned long IfStatement::labelCounter = 0;
unsigned long ForStatement::labelCounter = 0;
unsigned long WhileStatement::labelCounter = 0;
unsigned long DoWhileStatement::labelCounter = 0;
unsigned long int Block::blockCounter = 0;

map<SymbolType*, int> typePriority;
map<Values, SymbolType*> operationTypeOperands;
map<Values, SymbolType*> operationReturningType;

Node::Node() : token(0), nodeType(0), commonNode(0), replaced(0), opt(false), index(0), needDeleteng(false) {}

Node::Node(Token *t) : token(t), nodeType(0), replaced(0), opt(false), index(0), needDeleteng(false) {}

Node *Node::makeTypeCoerce(Node *expr, SymbolType *from, SymbolType *to){
	if(!from->canConvertTo(to))
		throw MyException("Cannot perform conversion", expr->token);
	if(from == to || *from == to)
		return expr;
	if (!dynamic_cast<ScalarSymbol*>(from) || !dynamic_cast<ScalarSymbol*>(to))
		return expr;
	else {
		if(typePriority[to] - typePriority[from] == 1)
			return new CastNode(0, expr, to);
		return new CastNode(0, makeTypeCoerce(expr, from, IntType), FloatType);
	}
}

OpNode::OpNode(Token *op) : Node(op) {}

string OpNode::opName(Values v){
	if(v == point)
		return ".";
	if(v == arrow)
		return "->";
	static char operations[][4] = { "+", "-", "*", "/", "++", "--", "%", ">", "<", "==", "!=", "!", "~", "^", "&&",
		"||", "&", "|", ">=", "<=", ">>", "<<", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "~=", "?", ">>=", "<<="};
	return string(operations[v]);
}

BinaryOpNode::BinaryOpNode(Token *t, Node *l, Node *r) : OpNode(t), left(l), right(r) {
	if((l == 0) || (r == 0))
		throw MyException("Lost operand", t->line, t->start);
} 

bool BinaryOpNode::isAssignment(Values op){
	return op == assign || op == pl_eql || op == mn_eql ||
		op == fct_eql || op ==div_eql || op == per_eql ||
		op == and_eql || op == or_eql || op == xor_eql ||
		op == sr_eql || op == sl_eql;
}

bool BinaryOpNode::isComparison(Values op){
	return op == equally || op == Values::less || op == more ||
		op == more_or_eql || op == less_or_eql || op == l_not;
}

bool BinaryOpNode::isConst() const {
	return left->isConst() && right->isConst();
}

Node* BinaryOpNode::calculate(){
	Values value = token->value;
	IntNumber *l_intToken = dynamic_cast<IntNumber*>(left->token);
	FloatNumber *l_floatToken = dynamic_cast<FloatNumber*>(left->token);
	Char *l_charToken = dynamic_cast<Char*>(left->token);
	Node *res = 0;

	if(l_floatToken){
		float leftOp = l_floatToken->v2,
			rightOp = dynamic_cast<FloatNumber*>(right->token)->v2;
		switch(value){
		case plus:
			res = new FloatNode(new FloatNumber(left->token->start, 0, leftOp + rightOp), 0);
			break;
		case minus:
			res = new FloatNode(new FloatNumber(left->token->start, 0, leftOp - rightOp), 0);
			break;
		case factor:
			res = new FloatNode(new FloatNumber(left->token->start, 0, leftOp * rightOp), 0);
			break;
		case divis:
			res = new FloatNode(new FloatNumber(left->token->start, 0, leftOp / rightOp), 0);
			break;
		case equally:
			res = new IntNode(new IntNumber(left->token->start, 0, leftOp == rightOp));
			break;
		case not_eql:
			res = new IntNode(new IntNumber(left->token->start, 0, leftOp != rightOp));
			break;
		default:
			throw MyException("Operation \"" + opName(value) + "\" not allowed for these operands", left->token);
		}
	} else {
		int leftOp, rightOp, r;
		if(l_intToken){
			leftOp = l_intToken->v2;
			rightOp = dynamic_cast<IntNumber*>(right->token)->v2;
		}
		else {
			leftOp = l_charToken->v2;
			rightOp = dynamic_cast<Char*>(right->token)->v2;
		}
		auto op = operationReturningType[value];
		switch(value){
		case plus:
			r = leftOp + rightOp;
			break;
		case minus:
			r = leftOp - rightOp;
			break;
		case factor:
			r = leftOp * rightOp;
			break;
		case divis:
			if(rightOp == 0)
				throw MyException("Division by zero", token);
			r = leftOp / rightOp;
			break;
		case percent:
			r = leftOp % rightOp;
			break;
		case more:
			r = leftOp > rightOp;
			break;
		case Values::less:
			r = leftOp < rightOp;
			break;
		case equally:
			r = leftOp == rightOp;
			break;
		case not_eql:
			r = leftOp != rightOp;
			break;
		case xor:
			r = leftOp ^ rightOp;
			break;
		case l_and:
			r = leftOp && rightOp;
			break;
		case l_or:
			r = leftOp || rightOp;
			break;
		case b_and:
			r = leftOp & rightOp;
			break;
		case b_or:
			r = leftOp | rightOp;
			break;
		case more_or_eql:
			r = leftOp >= rightOp;
			break;
		case less_or_eql:
			r = leftOp <= rightOp;
			break;
		case sr:
			r = leftOp >> rightOp;
			break;
		case sl:
			r = leftOp << rightOp;
			break;
		}
		if(op == CharType)
			res = new CharNode(new Char(left->token->start, 0, r));
		else
			res = new IntNode(new IntNumber(left->token->start, 0, r));
	}
	return res;
}

GoToNode::GoToNode(Block *b) : Node(0), block(b), label(block->endLabel) {}

GoToNode::GoToNode(AsmArgLabel *jump) : block(0), label(jump) {}

void GoToNode::print(int deep) const {
	cout << string(deep * 2, ' ') << "goto: " << label->labelName << endl;
}

void BinaryOpNode::print(int deep) const {
	left->print(deep + 1);
	cout << string(deep * 2, ' ') << opName(token->value) << endl;
	right->print(deep + 1);
}

void BinaryOpNode::ReplaceDiv2Mult(){
	if(*token != divis && *token != div_eql){
		left->ReplaceDiv2Mult();
		right->ReplaceDiv2Mult();
		return;
	}
	if(right->isSimple()){
		if(nodeType == IntType){
			return;
		}
		token->value = token->value == divis ? factor : fct_eql;
		token->str = token->value == factor ? "*" : "=*";
		if(FloatNumber *f = dynamic_cast<FloatNumber*>(right->token))
			f->v2 = 1.0 / f->v2;
		else {
			FloatNumber *fNumber = new FloatNumber(right->token->line, right->token->start,
				1.0 / dynamic_cast<IntNumber*>(right->token)->v2);
			delete right->token;
			right->token = fNumber;
		}
	}

}

Node* BinaryOpNode::SelfCopy(){
	return new BinaryOpNode(token, left->SelfCopy(), right->SelfCopy());
}

bool BinaryOpNode::operator == (Node *node) const {
	BinaryOpNode *bNode = dynamic_cast<BinaryOpNode*>(node);
	return bNode && (
		token->value == node->value() &&
		*left == bNode->left && *right == bNode->right ||
		Associative() && *left == bNode->right && *right == bNode->left);
}

bool BinaryOpNode::ContainLvalueRecourse(VarSymbol *id) const {
	return isAssignment(token->value) && (
		left->isIdent() && dynamic_cast<IdentifierNode*>(left)->token->str == id->name || 
		left->ContainLvalueRecourse(id) || 
		right->ContainLvalueRecourse(id));
}

bool BinaryOpNode::Associative() const {
	Values val = token->value;
	return val == plus || val == factor ||
		val == equally || val == not_eql ||
		val == xor || val == l_and || 
		val == l_or || val == b_and ||
		val == b_or;
}

void BinaryOpNode::print(ofstream *f, int deep){
	left->print(f, deep + 1);
	*f << string(deep * 2, ' ') << opName(token->value) << endl;
	right->print(deep + 1);
}

bool BinaryOpNode::isLvalue() const {
	switch(token->value){
	
	case arrow:
	case point:
		return right->isLvalue();
	
	default:
		return isModifiableLvalue();
	}
}

void BinaryOpNode::SetIndex(int i){
	index = i;
	left->SetIndex(i);
	right->SetIndex(i);
}

void BinaryOpNode::ReplaceCallNode(FuncCallNode *what, Node *to){
	if(left == what){
		delete left;
		left = to;
	} else
		left->ReplaceCallNode(what, to);
	if(right == what){
		delete right;
		right = to;
	} else
		right->ReplaceCallNode(what, to);
}

vector<FuncCallNode*>* BinaryOpNode::GetInlineFunction(){
	auto v1 = left->GetInlineFunction(),
		v2 = right->GetInlineFunction();
	if(!(v1 || v2))
		return 0;
	if(v1 && v2){
		v1->reserve(v1->size() + v2->size());
		copy(v2->begin(), v2->end(), back_inserter(*v1));
		delete v2;
		return v1;
	}
	return v1 ? v1 : v2;
}

bool BinaryOpNode::isModifiableLvalue() const {
	switch(token->value){

	case assign:
	case pl_eql:
	case mn_eql:
	case fct_eql:
	case div_eql:
	case per_eql:
	case sl:
	case sr:
	case xor_eql:
	case and_eql:
	case or_eql:
		return left->isModifiableLvalue();

	case point:
	case arrow:
		return right->isModifiableLvalue();
	default:
		return false;
	}

}

SymbolType *BinaryOpNode::getType(){
	if(nodeType)
		return nodeType;
	SymbolType *leftType = left->getType();
	SymbolType *rightType = right->getType();
	if(rightType == VoidType)
		throw MyException("Type casting error", token);
	Values operation = token->value;
	SymbolType *maxType = 0;
	if(operationTypeOperands.count(operation))
		maxType = operationTypeOperands[operation];
	else
		maxType = typePriority[leftType] > typePriority[rightType] ? leftType : rightType;
	PointerSymbol *l_pointer = dynamic_cast<PointerSymbol*>(leftType);
	PointerSymbol *r_pointer= dynamic_cast<PointerSymbol*>(rightType);
	ArraySymbol *l_array = dynamic_cast<ArraySymbol*>(leftType);
	ArraySymbol *r_array = dynamic_cast<ArraySymbol*>(rightType);

	switch(operation){

	case per_eql:
	case or_eql:
	case and_eql:
	case xor_eql:
	case sr_eql:
	case sl_eql:
		if(!leftType->canConvertTo(IntType) || !rightType->canConvertTo(IntType))
			throw MyException("Expression must have integral type", token);

	case assign:

		if(leftType->isStruct() && *leftType == rightType){
			nodeType = leftType;
			return leftType;
		}

	case pl_eql:
	case mn_eql:
	case fct_eql:
	case div_eql:
		if(!left->isModifiableLvalue())
			throw MyException("Expression must be a modifiable lvalue", token);
		right = makeTypeCoerce(right, rightType, leftType);
		nodeType = rightType;
		return rightType;

	case point:
		if(!dynamic_cast<StructSymbol*>(leftType))
			throw MyException("Left operand of . must be a structure", token);
		nodeType = rightType;
		return rightType;

	case arrow:	
		if(!l_pointer || !dynamic_cast<StructSymbol*>(l_pointer->upType()))
			throw MyException("Left operand of -> must be of pointer-to-structure type", left->token);
		nodeType = rightType;
		return rightType;

	case Values::minus:
		if(l_pointer && r_pointer || l_array && r_array){
			if( l_pointer && r_pointer && (*l_pointer->pointer != r_pointer->pointer)
				|| l_array && r_array && (*l_array->type != r_array->type))
				throw MyException("Operand types are incompatible", token);
			nodeType = IntType;
			return IntType;
		}
		
	case Values::plus:
		if(l_pointer && r_pointer || l_array && r_array)
			throw MyException("Can't add two pointers");
		if(l_pointer || r_pointer)
			return l_pointer == 0 ? r_pointer : l_pointer;
		if(l_array || r_array){
			nodeType = new PointerSymbol(l_array == 0 ? r_array->type : l_array->type);
			return nodeType;
		}
		
	default:
		if(leftType->isStruct() || rightType->isStruct())
			throw MyException("Can't perform operation over two structures", token);
		if(typePriority[maxType] < max(typePriority[leftType], typePriority[rightType]))
			throw MyException("Invalid type of operands", token);
		left = makeTypeCoerce(left, leftType, maxType);
		right = makeTypeCoerce(right, rightType, maxType);
		if (operationReturningType.count(operation)){
			nodeType = operationReturningType[operation];
			return nodeType;
		}
		else{
			nodeType = maxType;
			return maxType;
		}
	}
}

UnaryOpNode::UnaryOpNode(Token *t, Node *n) : OpNode(t), operand(n){};

void GoToBinaryOpNode::print(int deep) const {
	BinaryOpNode::print(deep);
	cout << string(deep * 2, ' ') << "goto: " << label->name() << endl;
}

void UnaryOpNode::print(int deep) const {
	cout << string(deep * 2, ' ') << opName(token->value) << endl;
	operand->print(deep + 1);
}

void UnaryOpNode::print(ofstream *f, int deep) const {
	*f << string(deep * 2, ' ') << opName(token->value) << endl;
	operand->print(f, deep + 1);
}

bool UnaryOpNode::isLvalue() const {
	Values operation = token->value;
	return operation == factor || (operation == incr || operation == decr) && operand->isLvalue();
}

void UnaryOpNode::ReplaceCallNode(FuncCallNode *what, Node *to){
	if(operand == what){
		delete operand;
		operand = to;
	} else
		operand->ReplaceCallNode(what, to);
}

vector<FuncCallNode*>* UnaryOpNode::GetInlineFunction(){
	return operand->GetInlineFunction();
}

void UnaryOpNode::ReplaceDiv2Mult(){
	operand->ReplaceDiv2Mult();
}

static bool TypeLValue(const UnaryOpNode *n){
	UnaryOpNode *node = const_cast<UnaryOpNode*>(n);
	return node->getType()->isModifiableLvalue();
}

bool UnaryOpNode::operator == (Node *node) const {
	UnaryOpNode *uNode = dynamic_cast<UnaryOpNode*>(node);
	return uNode && token->value == node->value() && *operand == uNode->operand;
}

void UnaryOpNode::SetIndex(int i){
	index = i;
	operand->SetIndex(i);
}

Node* UnaryOpNode::SelfCopy(){
	return new UnaryOpNode(token, operand->SelfCopy());
}

bool UnaryOpNode::isModifiableLvalue() const {
	Values operation = token->value; 
	return (operation == factor || operation == decr || operation == incr) && TypeLValue(this);
}

bool UnaryOpNode::isConst() const {
	return operand->isConst();
}

bool UnaryOpNode::ContainLvalueRecourse(VarSymbol *id) const {
	return (token->value == incr || token->value == decr) && operand->token->str == id->name || 
		operand->ContainLvalueRecourse(id);
}

Node* UnaryOpNode::calculate(){
	Values value = token->value;
	IntNumber *intToken = dynamic_cast<IntNumber*>(operand->token);
	FloatNumber *floatToken = dynamic_cast<FloatNumber*>(operand->token);
	Char *charToken = dynamic_cast<Char*>(operand->token);
	if(!(intToken || floatToken || charToken))
		throw MyException("Wrong call of the optimizer (constants folding)");
	if(value == b_not){
		int t = intToken ? intToken->v2 : charToken->v2;
		return new IntNode(new IntNumber(token->start, 0, ~t));
	}
	float t = intToken ? intToken->v2 : floatToken ? floatToken->v2 : charToken->v2;
	switch(value){
	case l_not:
		return new IntNode(new IntNumber(token->start, 0, !t));
	case minus:
			if(floatToken)
				return new FloatNode(new FloatNumber(token->start, 0, -t), 0);
			else
				return new IntNode(new IntNumber(token->start, 0, -(int)t));
	default:
		return 0;
	}
}

SymbolType *UnaryOpNode::getType(){
	if(nodeType)
		return nodeType;
	SymbolType *type = operand->getType();
	Values operation = token->value;
	switch(operation){

	case factor:
			if(!dynamic_cast<PointerSymbol*>(type))
				throw MyException("Type of unary operation is not a pointer", token);
			nodeType = dynamic_cast<PointerSymbol*>(type)->pointer;
			return nodeType;
	
	case b_and:									
		if(!operand->isLvalue())							
			throw MyException("Expression must be a modifiable lvalue", token->line, token->start);
		nodeType = new PointerSymbol(type);
		return nodeType;
		
	case b_not:
		operand = makeTypeCoerce(operand, type, IntType);
		break;

	case l_not:
		if(dynamic_cast<StructSymbol*>(type))
			throw MyException("No operator \"!\" matches these operands operand types are: !" + type->name, token);
		break;

	case incr:
	case  decr:
		if(!operand->isModifiableLvalue())
			throw MyException("Expression must be a modifiable lvalue", token);
		break;
	case Values::minus:
		if(!type->canConvertTo(FloatType))
			throw MyException("Expression must have arithmetical type", token);
	}
	nodeType = type;
	return type;
}

PostfixUnaryOpNode::PostfixUnaryOpNode(Token *t, Node *op) : UnaryOpNode(t, op) {};

bool PostfixUnaryOpNode::isLvalue() const {
	return false;
}

bool PostfixUnaryOpNode::isModifiableLvalue() const {
	return false;
}

void PostfixUnaryOpNode::print(int deep) const {
	operand->print(deep);
	cout << string(deep * 2, ' ') << opName(token->value) << endl;
}

void PostfixUnaryOpNode::print(ofstream *f, int deep) const {
	operand->print(deep);
	*f << string(deep * 2, ' ') << opName(token->value) << endl;
}

IntNode::IntNode(Token *t) : Node(t) {}


void IntNode::print(int deep) const {
	cout << string(deep * 2, ' ') << dynamic_cast<IntNumber*>(token)->v2 << endl;
}

void IntNode::print(ofstream *f, int deep) const {
	*f << string(deep * 2, ' ') << dynamic_cast<IntNumber*>(token)->v2 << endl;
}

Node* IntNode::SelfCopy(){
	return new IntNode(token);
}

SymbolType *IntNode::getType(){
	nodeType = IntType;
	return IntType;
}

FloatNode::FloatNode(Token *t, int ind) : Node(t), index(ind) {}


void FloatNode::print(int deep) const {
	cout << string(deep * 2, ' ') << dynamic_cast<FloatNumber*>(token)->v2 << endl;
}

void FloatNode::print(ofstream *f, int deep) const {
	*f << string(deep * 2, ' ') << dynamic_cast<FloatNumber*>(token)->v2 << endl;
}

string FloatNode::constName() const {
	return "float" + to_string(index);
}

void FloatNode::generate(AsmCode &code){
	code.add(cmdPUSH, makeArgMemory(constName()));
}

void FloatNode::generateLvalue(AsmCode &code){
	code.add(cmdPUSH, makeArgMemory(constName(), true));
}

SymbolType *FloatNode::getType(){
	nodeType = FloatType;
	return FloatType;
}

Node* FloatNode::SelfCopy(){
	return new FloatNode(token, 0);
}

IdentifierNode::IdentifierNode(Token *t, VarSymbol *v) : Node(t), var(v), assignCounter(0) {}

IdentifierNode::IdentifierNode(const string &name, int line, int col) :
	Node(new Identifier(line, col, name)), assignCounter(0) {}

IdentifierNode::IdentifierNode(const string &name, VarSymbol *v) : Node(new Identifier(0, 0, name)), 
	assignCounter(0), var(v) {}

IdentifierNode::IdentifierNode(VarSymbol *v) : Node(new Identifier(0, 0, v->name)), var(v), assignCounter(0) {}

SymbolType* IdentifierNode::getType(){
	if(nodeType)
		return nodeType;
	nodeType = var->getType();
	return nodeType;
}

bool IdentifierNode::isLvalue() const {
	return true;
}

Node* IdentifierNode::SelfCopy(){
	return new IdentifierNode(var);
}

Node* IdentifierNode::calculate(){
	if(!var->type->isConstSymbol())
		throw MyException("Wrong call of high-level optimizer (const folding)", token);
	return var->init;
}

bool IdentifierNode::operator == (Node *node) const {
	IdentifierNode *id = dynamic_cast<IdentifierNode*>(node);
	return id && var->name == id->var->name && assignCounter == id->assignCounter;
}

bool IdentifierNode::isModifiableLvalue() const {
	SymbolType *type = dynamic_cast<VarSymbol*>(var)->type;
	return !dynamic_cast<ConstSymbolType*>(type) && !dynamic_cast<FuncSymbol*>(type)
		&& !dynamic_cast<StructSymbol *>(type) && !dynamic_cast<ArraySymbol*>(type);
}

void IdentifierNode::generate(AsmCode &code) {
	int size = var->byteSize();
	int steps = size / 4 + (size % 4 != 0);
	if (var->global)
		for (int i = 0; i < steps; i++)
			code.add(cmdPUSH, makeArgMemory("dword ptr [var_" + var->name + " + " + to_string(4 * (steps - i - 1)) +"]"));
	else
		for (int i = 0; i < steps; i++)
			code.add(cmdPUSH, makeIndirectArg(EBP, var->offset + 4 * (steps - i - 1)));
}

void IdentifierNode::generateLvalue(AsmCode &code) {
	if(var->global)
		code.add(cmdPUSH, makeArgMemory("var_" + var->name, true));
	else
		code.add(cmdMOV, EAX, EBP)
			.add(cmdMOV, EBX, var->offset)
			.add(cmdADD, EAX, EBX)
			.add(cmdPUSH, EAX);
}

void IdentifierNode::print(int deep) const {
	cout << string(deep * 2, ' ') << token->str << endl;
}

void IdentifierNode::print(ofstream *f, int deep) const {
	*f << string(deep * 2, ' ') << token->str << endl;
}

CharNode::CharNode(Token *t) : Node(t) {}

SymbolType *CharNode::getType(){
	nodeType = CharType;
	return CharType;
}

Node* CharNode::SelfCopy(){
	return new CharNode(token);
}

void CharNode::print(int deep) const {
	cout << string(deep * 2, ' ') << '\'' << dynamic_cast<Char*>(token)->v2 << '\'' << endl;
}

void CharNode::print(ofstream *f, int deep) const {
	*f << string(deep * 2, ' ') << '\'' << dynamic_cast<Char*>(token)->v2 << '\'' << endl;
}

StringNode::StringNode(Token *t, int ind) : Node(t), index(ind) {}

SymbolType* StringNode::getType(){
	nodeType = StringType;
		return StringType;
}

Node* StringNode::SelfCopy(){
	return new StringNode(token, 0);
}


void StringNode::print(int deep) const { 
	cout << string(deep * 2, ' ') << '"' << dynamic_cast<String*>(token)->str << '"' << endl; 
}

void StringNode::print(ofstream *f, int deep) const { 
	*f << string(deep * 2, ' ') << '"' << dynamic_cast<String*>(token)->str << '"' << endl; 
}

ArrNode::ArrNode(Node* arr): FunctionalNode(arr) {}

bool ArrNode::isLvalue() const {
	return true;
}

bool ArrNode::isModifiableLvalue() const {
	SymbolType *type = name->getType();
	for(int i = 0; i < args.size(); i++)
		type = type->upType();
	return type->isModifiableLvalue();
}

SymbolType *ArrNode::getType() {
	if(nodeType)
		return nodeType;
	ArraySymbol *sym = dynamic_cast<ArraySymbol*>(name->getType());
	if(!sym)
		throw MyException("Expression must be a pointer type", token);
	SymbolType *type = sym;
	for(int i = 0; i < args.size(); i++){
		type = type->upType();
		if(type == 0)
			throw MyException("Expression must be a pointer type", args[i]->token);
		if(!args[i]->getType()->canConvertTo(IntType))
			throw MyException("Expression must have integral type", args[i]->token);
		args[i] = makeTypeCoerce(args[i], args[i]->getType(), IntType);
	}
	nodeType = type;
	return type;
}

void ArrNode::print(int deep) const {
	name->print(deep);
	cout << string(deep * 2, ' ') << "[" << endl;
	printArgs(deep);
	cout << string(deep * 2, ' ') << "]" << endl;
}

void ArrNode::print(ofstream *f, int deep) const {
	name->print(f, deep);
	*f << string(deep * 2, ' ') << "[" << endl;
	printArgs(f, deep);
	*f<< string(deep * 2, ' ') << "]" << endl;
}

TernaryOpNode::TernaryOpNode(Token* op, Node* c, Node* l, Node* r): BinaryOpNode(op, l, r), condition(c) {}

Node* TernaryOpNode::calculate() {
	if(*condition->token == T_INTEGER){
		if(dynamic_cast<IntNumber*>(condition->token)->v2)
			return left;
		else return right;
	} else
	if(*condition->token == T_FLOAT){
		if(dynamic_cast<FloatNumber*>(condition->token)->v2)
			return left;
		else 
			return right;
	} else
	if(*condition->token == T_CHAR){
		if(dynamic_cast<Char*>(condition->token)->v2)
			return left;
		else
			return right;
	} else
		throw MyException("Wrong call of high-level optimizer (const folding)", token);
}

bool TernaryOpNode::isTernaryOpNode() const{
	return true;
}

void TernaryOpNode::print(int deep) const {
	string tab = string(deep * 2, ' ');
	cout << tab << "(" << endl;
	condition->print(deep + 1);
	cout << tab << ")" << endl;
	cout << tab << "?" << endl;
	left->print(deep + 1);
	cout << tab << ":" << endl;
	right->print(deep + 1);
}

void TernaryOpNode::ReplaceDiv2Mult() {
	condition->ReplaceDiv2Mult();
	BinaryOpNode::ReplaceDiv2Mult();
}

Node* TernaryOpNode::SelfCopy(){
	return new TernaryOpNode(token, condition->SelfCopy(), left->SelfCopy(), right->SelfCopy());
}

void TernaryOpNode::ReplaceCallNode(FuncCallNode *what, Node *to){
	BinaryOpNode::ReplaceCallNode(what, to);
	if(condition == what){
		delete condition;
		condition = to;
	} else
		condition->ReplaceCallNode(what, to);
}

vector<FuncCallNode*>* TernaryOpNode::GetInlineFunction() {
	auto v1 = BinaryOpNode::GetInlineFunction(),
		 v2 = condition->GetInlineFunction();
	if(!(v1 || v2))
		return 0;
	if(v1 && v2){
		v1->reserve(v1->size() + v2->size());
		copy(v2->begin(), v2->end(), back_inserter(*v1));
		delete v2;
		return v1;
	}
	return v1 ? v1 : v2;
}

void TernaryOpNode::print(ofstream *f, int deep) const {
	string tab = string(deep * 2, ' ');
	*f << tab << "(" << endl;
	condition->print(f, deep + 1);
	*f << tab << ")" << endl;
	*f << tab << "?" << endl;
	left->print(f, deep + 1);
	*f << tab << ":" << endl;
	right->print(f, deep + 1);
}

bool TernaryOpNode::isConst() const {
	return condition->isConst();
}

FunctionalNode::FunctionalNode(Node *n) : name(n), args(0) {}

FunctionalNode::FunctionalNode(Token *t, Node *n) : Node(t), name(n) {}

void FunctionalNode::ReplaceDiv2Mult() {
	for(Node *node : args)
		node->ReplaceDiv2Mult();
}

Node* FunctionalNode::SelfCopy(){
	FunctionalNode *fNode = new FunctionalNode(token, name->SelfCopy());
	for(Node *arg : args)
		fNode->args.push_back(arg->SelfCopy());
	return fNode;
}

void FunctionalNode::ReplaceCallNode(FuncCallNode *what, Node *to){
	for(int i = 0; i < args.size(); i++){
		if(args[i] == what){
			delete args[i];
			args[i] = to;
			return;
		} else
			args[i]->ReplaceCallNode(what, to);
	}
}

vector<FuncCallNode*>* FunctionalNode::GetInlineFunction(){
	vector<FuncCallNode*> *res = new vector<FuncCallNode*>; 
	for(int i = 0; i < args.size(); i++){
		auto v = args[i]->GetInlineFunction();
		if(v)
			copy(v->begin(), v->end(), back_inserter(*res));
	}
	if(res->size() == 0){
		delete res;
		return 0;
	}
	return res;
}	

void FunctionalNode::SetIndex(int i){
	index = i;
	for(Node *arg : args)
		arg->SetIndex(i);
}

void FuncCallNode::print(int deep) const {
	name->print(deep);
	cout << string(deep * 2, ' ') << "(" << endl;
	printArgs(deep);
	cout << string(deep * 2, ' ') << ")" << endl;
}

vector<FuncCallNode*>* FuncCallNode::GetInlineFunction() {
	if(!symbol->Inline)
		return 0;
	vector<FuncCallNode*> *ret = new vector<FuncCallNode*>;
	ret->push_back(this);
	return ret;
}

bool FuncCallNode::isRecursiveFuncCallNode() const{
	return symbol->UseRecursion();
}

bool FuncCallNode::isInlineSubstPossibe() const {
	for(Node *expr : symbol->body->body)
		for(int i = 0; i < symbol->params->sym_ptr.size(); i++)
			if(expr->ContainLvalueRecourse(dynamic_cast<VarSymbol*>(symbol->params->sym_ptr[i]))
				&& !args[i]->isModifiableLvalue())
				return false;
		return true;
}

bool FuncCallNode::isVoidFuncCallNode() const{
	return symbol->value->name == "void";
}

SymbolType* FuncCallNode::getType(){
	if(nodeType)
		return nodeType;
	FuncSymbol* sym = dynamic_cast<FuncSymbol*>(symbol);
	int formalParametersCount = sym->params->size();
	int realParametersCount = args.size();
	if (formalParametersCount != realParametersCount)
		throw MyException("Incorrect parameters count", token);
	for (int i = 0; i < formalParametersCount; i++)
	{
		SymbolType* realParamType = args[i]->getType();
		SymbolType* formalParamType = sym->params->sym_ptr[i]->getType();
		if (!realParamType->canConvertTo(formalParamType))
			throw MyException("Invalid type of parameter", args[i]->token);
		args[i] = makeTypeCoerce(args[i], realParamType, formalParamType);
	}
	return symbol->getType();
}

void FunctionalNode::addArg(Node* arg) { args.push_back(arg); }

SymbolType* IONode::getType(){
	return 0;
}

void IONode::print(int deep) const {

	string tab(deep * 2, ' ');
	cout << tab << token->str << "(" << endl;
	format->print(deep + 1);
	if (args.size() > 0)
	{
		cout << tab << ',' << endl;
		printArgs(deep + 1);
	}	
	cout << tab << ")" << endl;
}

void FunctionalNode::printArgs(int deep) const {
	for (int i = 0; i < args.size(); i++){
		args[i]->print(deep + 1);
		if (i < args.size() - 1)
			cout << string(deep * 2, ' ') << ',' << endl;
	}
}

void FunctionalNode::printArgs(ofstream *f, int deep) const {
	for (int i = 0; i < args.size(); i++){
		args[i]->print(f, deep + 1);
		if (i < args.size() - 1)
			*f << string(deep * 2, ' ') << ',' << endl;
	}
}

void FunctionalNode::print(ofstream *f, int deep) const {
	name->print(f, deep);
	*f << string(deep * 2, ' ') << "[" << endl;
	printArgs(f, deep);
	*f<< string(deep * 2, ' ') << "]" << endl;
}

void FunctionalNode::print(int deep) const {
	name->print(deep);
	cout << string(deep * 2, ' ') << "(" << endl;
	printArgs(deep);
	cout << string(deep * 2, ' ') << ")" << endl;
}

bool FuncSymbol::UseRecursion() const {
	for(Node *expr : body->body){
		if(FuncCallNode *cNode = dynamic_cast<FuncCallNode*>(expr))
			if(this == cNode->symbol)
				return true;
	}
	return false;
}

CastNode::CastNode(Token *op, Node *oper, SymbolType *ts) : UnaryOpNode(op, oper), s_type(ts) {}

Node* CastNode::calculate(){
	if(!operand->isSimple())
		throw MyException("Wrong call of high-level optimizer (const folding)", token);

	if(s_type == IntType){
		if(*operand->token == T_INTEGER)
			return operand;
		else 
			if(*operand->token == T_FLOAT){
				FloatNumber *f = dynamic_cast<FloatNumber*>(operand->token);
				return new IntNode(new IntNumber(f->line, f->start, (int)f->v2));
			}
		else 
			if(*operand->token == T_CHAR){
				Char *ch = dynamic_cast<Char*>(operand->token);
				return new IntNode(new IntNumber(ch->line, ch->start, (int)ch->v2));
			}
	}

	if(s_type == FloatType){
		if(*operand->token == T_FLOAT)
			return operand;
		else
			if(*operand->token == T_INTEGER){
				IntNumber *i = dynamic_cast<IntNumber*>(operand->token);
				return new FloatNode(new FloatNumber(i->line, i->start, (float)i->v2), 0);
			}

		else 
			if(*operand->token == T_CHAR){
				Char *ch = dynamic_cast<Char*>(operand->token);
				return new FloatNode(new FloatNumber(ch->line, ch->start, (float)ch->v2), 0);
			}
		}

	if(s_type == CharType){
		if(*operand->token == T_CHAR)
			return operand;
		else
			if(*operand->token == T_INTEGER){
				IntNumber *i = dynamic_cast<IntNumber*>(operand->token);
				return new CharNode(new Char(i->line, i->start, (char)i->v2));
			}
		else
			if(*operand->token == T_FLOAT){
				FloatNumber *f = dynamic_cast<FloatNumber*>(operand->token);
				return new CharNode(new Char(f->line, f->start, (char)f->v2));
			}
		}
	throw MyException("Wrong call of high-level optimizer (const folding)", token);
}

SymbolType *CastNode::getType(){
	nodeType = s_type;
	return s_type;
}

void CastNode::print(int deep) const {
	cout << string(deep * 2, ' ') << s_type->name;
	cout << string(deep * 2, ' ') << "(" << endl;
	operand->print(deep + 1);
	cout << string(deep * 2, ' ') << ")" << endl;
}

void CastNode::print(ofstream *f, int deep) const {
	*f << string(deep * 2, ' ') << s_type->name;
	*f << string(deep * 2, ' ') << "(" << endl;
	operand->print(f, deep + 1);
	*f << string(deep * 2, ' ') << ")" << endl;
}

void IfStatement::print(int deep) const {
	string tab(deep * 2, ' '); 
	cout << tab << "if" << endl << endl;
	condition->print(deep + 1);
	cout << endl << tab << "then" << endl << endl;
	if_branch->print(deep + 1);
	if(else_branch){
		cout << tab << "else" << endl << endl;
		else_branch->print(deep + 1);
	}
}

Node* IfStatement::SelfCopy(){
	return new IfStatement(condition->SelfCopy(), new Block(*dynamic_cast<Block*>(if_branch)),
		new Block(*dynamic_cast<Block*>(else_branch)));
}

void CycleStatement::ReplaceDiv2Mult(){
	body->ReplaceDiv2Mult();
}

Node* ForStatement::SelfCopy(){
	Node * node = body->SelfCopy();
	return new ForStatement(first_cond->SelfCopy(), second_cond->SelfCopy(), third_cond->SelfCopy(), node);
}

void ForStatement::print(int deep) const {
	string tab(deep * 2, ' ');
	cout << tab << "for (;;) .." << endl;
	cout << tab << "First action:" << endl;
	if(first_cond)
		first_cond->print(deep + 1);
	else
		cout << tab <<"none" << endl;
	cout << tab << "Stop condition:" << endl;
	if(second_cond)
		second_cond->print(deep + 1);
	else
		cout << tab << "none" << endl;
	cout << tab << "Step:" << endl;
	if(third_cond)
		third_cond->print(deep + 1);
	else
		cout << tab << "none" << endl;
	cout << tab << "Body:" << endl;
	if(body)
		body->print(deep + 1);
	else
		cout << tab << "Block is empty" << endl;
}

static void print_cycle(int deep, Node *condition, Node *body){
	string tab(deep * 2, ' ');
	cout << tab << "Condition:" << endl;
	condition->print(deep + 1);
	cout << tab << "Body:" << endl;
	if(body)
		body->print(deep + 1);
	else
		cout << tab << "<< block is empty >>" << endl;
}

Node* WhileStatement::SelfCopy(){
	return new WhileStatement(condition->SelfCopy(), body->SelfCopy());
}

void WhileStatement::print(int deep) const {
	cout << string(deep * 2, ' ') << "while () .." << endl;
	print_cycle(deep, condition, body);
}

Node* DoWhileStatement::SelfCopy(){
	return new DoWhileStatement(condition->SelfCopy(), body->SelfCopy());
}

void DoWhileStatement::print(int deep) const {
	cout << string(deep * 2, ' ') << "do .. while()" << endl;
	print_cycle(deep, condition, body);
}

Node* ContinueStatement::SelfCopy(){
	return new ContinueStatement(owner);
}


void ContinueStatement::print(int deep) const {
	cout << string(deep * 2, ' ') << "continue" << endl;
}

void BreakStatement::print(int deep) const {
	cout << string(deep * 2, ' ') << "break" << endl;
}

Node* BreakStatement::SelfCopy(){
	return new BreakStatement(owner);
}

void ReturnStatement::print(int deep) const {
	cout << string(deep * 2, ' ') << "return" << endl;
	if(value)
		value->print(deep + 1);
}

Node* ReturnStatement::SelfCopy(){
	Node *val = value ? value->SelfCopy() : 0;
	return new ReturnStatement(val, owner);
}

int Block::size() const {
	return body.size();
}

void SymTable::CreateCallParams(){

	callParams.resize(sym_ptr.size());
	static const string paramName("param");	
	for(int i = 0; i < sym_ptr.size(); i++)
		callParams[i] = new IdentifierNode(paramName + to_string(i), dynamic_cast<VarSymbol*>(sym_ptr[i]));
}

void SymTable::InitCallParams(vector<Node*> &params, Block *block){
	if(!callParams.size())
		return;
	vector<Node*> p;
	for(int i = 0; i < callParams.size(); i++){
		p.push_back(new BinaryOpNode(new Operation(assign), callParams[i], params[i]));
		VarSymbol *s = new VarSymbol(*dynamic_cast<IdentifierNode*>(callParams[i])->var);
		s->name = callParams[i]->token->str;
		block->table->sym_ptr.push_back(s);
	}
	
	copy(block->body.begin(), block->body.end(), back_inserter(p));
	block->body = p;
}

Block::Block(const Block &block, SymTable *s_table) : table(block.table), blockInserter(0), copy(true){
	for(int i = 0; i < block.body.size(); i++)
		body.push_back(block.body[i]->SelfCopy());
	endLabel =  new AsmArgLabel("BLOCK_LABEL" + to_string(Block::blockCounter++));
	if(s_table){
		for(int i = 0; i < table->sym_ptr.size(); i++){
			s_table->sym_ptr.push_back(table->sym_ptr[i]);
		}
	}
}


void Block::ReplaceDiv2Mult(){
	for(int i = 0; i < size(); i++)
		body[i]->ReplaceDiv2Mult();
}

void Block::initSubExprBlock(SubExpressionBlock *e){
	exprBlock = e;
}

SubExpressionBlock* Block::GetExprBlock() const {
	return exprBlock;
}

Node* Block::SelfCopy(){
	return new Block(*this);
}

void Block::print(int deep) const {
	string tab(deep * 2 + 2, ' ');
	const static string separator("--------------------------\n");
	cout << string(deep * 2, ' ') << '{' << endl;
	if(body.size())
		cout << tab << "Statements:" << endl;
	for(int i = 0; i < body.size(); i++){
		body[i]->print(deep + 1);
		cout << separator;
	}
	if(copy)
		cout << string(deep * 2, ' ') << endLabel->labelName << ':' << endl;
	
	if(table->sym_ptr.size()){
		cout << tab << "Symbols Table:" << endl;
		table->print(deep + 1);		
	}
	if(!table->sym_ptr.size() && !body.size())
		cout << tab << "<< block is empty >>" << endl;
	cout << string(deep * 2, ' ') << '}' << endl;
}

void func_print(int deep, Block *b){
	b->print(deep + 1);
}

void print_node(int deep, Node *n){
	n->print(deep + 1);
}

extern void Push_Float(FloatNode*);

 Node* init_var(SymbolType *type){
	 if(dynamic_cast<ScalarSymbol*>(type) == IntType)
		 return new IntNode(new IntNumber(0, 0, "0"));
	 if(dynamic_cast<ScalarSymbol*>(type) == FloatType){
		FloatNode *ret = new FloatNode(new FloatNumber(0, 0, "0"), floatCount++);
		Push_Float(ret);
		return ret;
	 }
	 if(dynamic_cast<ScalarSymbol*>(type) == CharType)
		 return new CharNode(new Char(0, 0, '0'));
 }