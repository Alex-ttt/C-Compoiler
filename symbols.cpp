#include "symbols.h"
#include <string>

using namespace std;

void Symbol::print(int deep) const {
	cout << string(2 * deep, ' ') << name << endl;
}

string SymbolType::typeName() const {
	return name;
}

extern void print_node(int, Node*);

SymbolType* VarSymbol::getType(){
	return type;
}

VarSymbol::VarSymbol(const string &str, SymbolType *tp, Node *i) : 
	SymbolType(str), type(tp), init(i), global(true), assignCounter(0), lastUsing(0){
	if(!init)
		init = init_var(type);
}

int VarSymbol::byteSize() const {
	return type->byteSize();
}

void VarSymbol::generate(AsmCode &code) const {
	int size = type->byteSize();
	code.add(cmdDD, makeArgMemory("var_" + name), makeArgDup(size / 4 + (size % 4 != 0)));
}

void VarSymbol::print(int deep) const {
	cout << string(2 * deep, ' ') << name;
	if(type->isFunc())
		type->print(deep);
	else
		cout << ' ' << type->typeName() << endl;
}

bool VarSymbol::GetInline() const {
	FuncSymbol *fSymb = dynamic_cast<FuncSymbol*>(type);
	return fSymb && fSymb->value->GetInline();
}

bool VarSymbol::isFuncVar() const {
	return dynamic_cast<FuncSymbol*>(type);}

bool ScalarSymbol::canConvertTo(SymbolType *to){
	
	if(dynamic_cast<PointerSymbol*>(to) || dynamic_cast<FuncSymbol*>(to))
		return false;
	int a = typePriority[this];
	int b = typePriority[to];
	return a <= b;
}

bool ScalarSymbol::isModifiableLvalue() const {
	return true;
}

int ScalarSymbol::byteSize() const {
	if(name == "void")
		return 0;
	if(name == "char")
		return 1;
	if(name == "int" || name == "float")
		return 4;
	throw MyException("Unknown basic type");
}

string ConstSymbolType::typeName() const {
	return "const " + symbol->typeName();
}

bool ConstSymbolType::canConvertTo(SymbolType* to){
	return symbol->canConvertTo(to);
}

extern void func_print(int, Block*);

FuncSymbol::FuncSymbol(const string &name, SymbolType *val, Block *block)
	: SymbolType(name, val->Inline), value(val), body(block), params(0), endLabel(0) {}
	
bool FuncSymbol::isFunc() const {
	return true;
}

void FuncSymbol:: print(int deep) const {
	cout << string(2 * deep,' ') << value->typeName() << " function " << name << "(\n";
	params->print(deep + 1);
	cout <<string (deep + 1, ' ')<< ")" << endl;
	if(body)
		func_print(deep, body);
}

SymbolType* FuncSymbol::getType(){
	return value;
}

bool FuncSymbol::canConvertTo(SymbolType* to){
	return *this == to;
}

string ArraySymbol::typeName() const {
	char buff[10];
	_itoa_s(size, buff, 10);
	string massage = "array[";
	massage += buff;
	return massage + "] of " + type->typeName();
}

bool ArraySymbol::operator == (SymbolType* s) const {
	ArraySymbol *arr = dynamic_cast<ArraySymbol*>(s);
	if(!arr)
		return false;
	return size == arr->size && *type == arr->type;
}

bool ArraySymbol::operator != (SymbolType *s) const {
	return !(*this == s);
}


bool ArraySymbol::canConvertTo(SymbolType *to){
	if(*to == IntType)
		return true;
	PointerSymbol *ptr = dynamic_cast<PointerSymbol*>(to);
	return ptr && (*ptr->pointer == type);
}

int ArraySymbol::byteSize() const {
	return type->byteSize() * size;
}

PointerSymbol* ArraySymbol::convertToPointer() const {
	return new PointerSymbol(type);
}


bool FuncSymbol::operator == (SymbolType *s) const {
	FuncSymbol *f = dynamic_cast<FuncSymbol*>(s);
	return f && params == f->params && *value == f->value;
}


bool FuncSymbol::operator != (SymbolType *s) const {
	return !(*this == s);
}


bool PointerSymbol::operator == (SymbolType *s) const {
	PointerSymbol *p = dynamic_cast<PointerSymbol*>(s);
	if(!p)
		return false;
	SymbolType *type1 = pointer;
	SymbolType *type2 = p->upType();
	
	while(true){
		if(*type2 == "void" && dynamic_cast<ScalarSymbol*>(type1))
			return true;
		if(!(*type1 == type2))
			return false;
		SymbolType *t1 = type1->upType();
		SymbolType *t2 = type2->upType();
		if(!(t1 && t2))
			return !(t1 != 0 && t2 != 0);
		else
		{
			type1 = t1;
			type2 = t2;
		}
	}
}

bool PointerSymbol::canConvertTo(SymbolType *to){
	if(*to == IntType)
		return true;
	PointerSymbol *pointer = dynamic_cast<PointerSymbol*>(to);
	if(pointer)
		return *this == pointer;
	return false;
}


bool PointerSymbol::isModifiableLvalue() const {
	return true;
}

bool PointerSymbol::operator != (SymbolType *s) const {
	return !(*this == s);
}
string PointerSymbol::typeName() const {
	return "pointer to " + pointer->typeName();
}

int PointerSymbol::byteSize() const {
	return 4;
}

bool StructSymbol::canConvertTo(SymbolType *to){
	StructSymbol* _struct = dynamic_cast<StructSymbol*>(to);
	if (!_struct || (*s_table != _struct->s_table))
		return false;
	return true;
}


string StructSymbol::typeName() const {
	return "struct " + name;
}

int StructSymbol::byteSize() const {
	return s_table ? s_table->byteSize() : 0; 
}

SymbolType* StructSymbol::upType(){
	return (*s_table)[0]->getType();
}

int StructSymbol::getShiftForBase() const {
	SymbolType *t = (*s_table)[0]->getType();
	while(!dynamic_cast<ScalarSymbol*>(t))
		t = t->upType();
	return t->byteSize();
}

void StructSymbol::print(int deep) const {
	cout << string(2 * deep, ' ') << "struct " << name << endl;
	if (s_table)
		s_table->print(deep + 1);
}

void SymTable::print(int deep) const {
	for(int i = 0; i < name.size(); i++){
		cout << string(2 * deep, ' ');
		sym_ptr[i]->print(deep + 1);	
	}
}

Symbol* SymTable::find_symbol(const string &name) const {
	return index.count(name) == 1 ? sym_ptr[index.at(name)] : 0;
}

Symbol* SymTable::find_type(const string &name) const {
	Symbol *symb = find_symbol(name);
	if(dynamic_cast<VarSymbol*>(symb))
		return 0;
	else 
		return symb;
}

Symbol* SymTable::back() const {
	return sym_ptr.back();
}

int SymTable::byteSize() const {
	int b = 0;
	for(int i = 0; i < size(); i++)
		b += sym_ptr[i]->byteSize();
	return b;	
}

int SymTable::size() const {
	return sym_ptr.size();
}

bool SymTable::exists(const string& name) const {
	return find_symbol(name) != 0;
}


void SymTable::generateGlobals(AsmCode &code) const {
	for(int i = 0; i < size(); i++){
		VarSymbol *sym = dynamic_cast<VarSymbol*>(sym_ptr[i]);
		if(sym && !sym->type->isFunc())
			sym->generate(code);
	}
}

void SymTable::generateCode(AsmCode &code) const {
	for(int i = 0; i < size(); i++){
		VarSymbol* sym = dynamic_cast<VarSymbol*>(sym_ptr[i]);
		if (sym && dynamic_cast<FuncSymbol*>(sym->type))
			dynamic_cast<FuncSymbol*>(sym->type)->generate(code, sym->name);
	}
}

void SymTable::add_symbol(Symbol *s){
		name.push_back(s->name);
		sym_ptr.push_back(s);
		index[s->name] = name.size() - 1;
}

bool SymTable::isExist(const string &name) const {
	return find_symbol(name) != 0;
}

bool SymTable::operator == (SymTable* table) const {
	if(sym_ptr.size() != table->sym_ptr.size())
		return false;
	for(int i = 0; i < sym_ptr.size(); i++)
		if(*dynamic_cast<VarSymbol*>(sym_ptr[i])->type != dynamic_cast<VarSymbol*>(table->sym_ptr[i])->type)
			return false;
	return true;
}

bool SymTable::operator != (SymTable* table) const {
	return !(*this == table);
}


void SymTableForLocals::add_symbol(Symbol *s){
	SymTable::add_symbol(s);
	if(VarSymbol *var = dynamic_cast<VarSymbol*>(s)){
		SymbolType* type = var->getType();
		if (dynamic_cast<ArraySymbol*>(type))
			s->offset = -(shift + offset + type->byteSize() - type->upType()->byteSize());
		else if (dynamic_cast<StructSymbol*>(type))
			s->offset = -(shift + offset + type->byteSize() - dynamic_cast<StructSymbol*>(type)->getShiftForBase());
		else
			s->offset = -(shift + offset);
		offset += s->byteSize();
		var->global = false;
	}
}

void SymTableForParams::add_symbol(Symbol *s){
	SymTable::add_symbol(s);
	if (StructSymbol* strct = dynamic_cast<StructSymbol*>(s->getType()))
		s->offset = offset + strct->getShiftForBase();
	else 
		s->offset = offset + s->byteSize();	
	offset += s->byteSize();
}

void SymTableForFields::add_symbol(Symbol *s){
	SymTable::add_symbol(s);
	s->offset = offset;
	offset += s->byteSize();
}

SymTable* SymTableStack::top(){
	if(tables.empty())
		return 0;
	return tables.back();
}

Symbol* SymTable::operator [] (int i) const {
	return sym_ptr[i];
}

void SymTableStack::add_symbol(Symbol* s){
	top()->add_symbol(s);
}

void SymTableStack::push(SymTable *t){
	tables.push_back(t);
}

bool SymTableStack::exist_in_top(const string &name){
	return top()->find_symbol(name) != 0;
}

void SymTableStack::pop(){
	tables.pop_back();
}

void SymTableStack::print(int deep) const {
	static const string line = "\n   ----=====================================================----\n";
	for(int i = 1; i < tables.size(); i++){
		tables[i]->print(deep);
		cout << line;
	}
}

Symbol* SymTableStack::find_symbol(const string &name) const {
	Symbol* sym = 0;
	for (int i = tables.size() - 1; i >= 0 && !sym; i--)
		sym = tables[i]->find_symbol(name);
	return sym;
}

Symbol* SymTableStack::find_type(const string &name) const {
	Symbol* sym = 0;
	for (int i = tables.size() - 1; i >= 0 && !sym; i--)
		sym = tables[i]->find_type(name);
	return sym;
}

int SymTableStack::size() const {
	return tables.size();
}
