#include "commands.h"
#include "generator.h"

#include <fstream>

string AsmImmediateArg::generate() const {
	return to_string(value);
}

bool AsmImmediateArg::operator == (int v) const {
	return value == v;
}

bool AsmImmediateArg::isImmediate() const {
	return true;
}

string AsmStringArg::generate() const {
	return value;
}

string AsmRegister::get_str() const {
	static const string arr[] = {"eax", "ebx", "ecx", "edx", "ebp", "esp", "cl", "al", "bl", "ax"};
	return arr[reg];
}

string AsmRegister::generate() const {
	return get_str();
}

bool AsmRegister::usedRegister(Registers r) const {
	return reg == r;
}

bool AsmRegister::operator == (AsmArg *a) const {
	AsmRegister *r = dynamic_cast<AsmRegister*>(a);
	return r && r->reg == reg && !dynamic_cast<AsmIndirectArg*>(a);
}

bool AsmRegister::operator == (Registers r) const {
	return r == reg;
}

bool AsmRegister::isRegister() const {
	return true;
}

string AsmIndirectArg::generate() const {
	return "dword ptr [" + get_str() + " + " + to_string(offset) + "]"; 
}

bool AsmIndirectArg::operator == (AsmArg *a) const {
	AsmIndirectArg* r = dynamic_cast<AsmIndirectArg*>(a);
	return r && r->reg == reg && r->offset == offset;
}

bool AsmIndirectArg::operator == (Registers r) const {
	return false;
}

bool AsmIndirectArg::usedRegister(Registers r) const {
	return r == reg;
}

bool AsmIndirectArg::isMemory() const {
	return true;
}

string AsmMemory::generate() const {
	return (lvalue ? "offset " : "") + varName;
}

bool AsmMemory::operator == (AsmArg *a) const {
	AsmMemory *m = dynamic_cast<AsmMemory*>(a);
	return m && m->varName == varName;
}

bool AsmMemory::isMemory() const {
	return true;
}

bool AsmMemory::isOffset() const {
	return lvalue;
}

void AsmMemory::clearOffset(){
	lvalue = false;
}

string AsmArgLabel::generate() const {
	return labelName;
}

string AsmArgLabel::name() const {
	return labelName;
}


bool AsmArgLabel::operator == (AsmArg *a) const {
	AsmArgLabel *l = dynamic_cast<AsmArgLabel*>(a);
	return l && l->labelName == labelName;
}

string AsmDup::generate() const {
	return to_string(count) + " dup(0)";
}

string AsmFloat::generate() const {
	return to_string(value);
}

bool AsmInstrLabel::operator == (const string &str) const {
	return label && label->labelName == str;
}

string AsmInstrLabel::generate() const {
	return label->labelName + ": ";
}

string AsmCmd::get_str() const {
	static const string arr[] = {
		"mov", "push", "pop", "mul", "imul", "div", "idiv", "add", "sub", "inc", "dec", "ret", "neg", "cdq",
		"invoke", "shr", "shl", "and", "or", "xor", "not", "call", "jmp", "cmp", "je", "jne", "setg", "setl",
		"setge", "setle", "sete", "setne", "fdivp", "faddp", "fmulp", "fsubp", "fld", "fstp", "fchs", "fild",
		"fistp", "fld1", "fcompp", "fnstsw", "sahf", "real4", "real8", "dd", "db", "dq"
	};
	return arr[operation];
}

string AsmCmd::generate() const {
	return get_str();
}

bool AsmCmd::operator == (Commands c) const {
	return operation == c;
}

string AsmCmd1::generate() const {
	return get_str() + (dynamic_cast<AsmImmediateArg*>(arg) && operation != cmdRET ? " dword ptr " : " ") 
		+ arg->generate();
}

AsmArg* AsmCmd1::argument() const {
	return arg;
}

bool AsmCmd1::changeStack() const {
	return operation == cmdPUSH || operation == cmdPOP || operation == cmdRET || operation == cmdCALL;
}

bool AsmCmd1::operationWith(AsmArg *a) const {
	AsmIndirectArg *indir = dynamic_cast<AsmIndirectArg*>(a);
	AsmRegister *r = dynamic_cast<AsmRegister*>(arg);
	if(indir && r)
		return indir->usedRegister(r->reg);
	else
		return *arg == a;
}

bool AsmCmd1::usedRegister(Registers r) const {
	return arg->usedRegister(r) || operation ==	cmdIDIV;
}

bool AsmCmd1::isJump() const {
	return operation == cmdJMP || operation == cmdJE || operation == cmdJNE || operation == cmdCALL;
}

bool AsmCmd1::operateWith(AsmArg* a) const {
	AsmIndirectArg* ind = dynamic_cast<AsmIndirectArg*>(a);
	AsmRegister* reg = dynamic_cast<AsmRegister*>(arg);
	return ind && reg ? ind->usedRegister(reg->reg) : *arg == a;
}

string AsmCmd2::generate() const {
	return operation < cmdREAL4 
		? get_str() + " " + arg1->generate() + ", " + arg2->generate() 
		: arg1->generate() + " " + get_str() + " " + arg2->generate();
}

AsmArg* AsmCmd2::firstArg() const {
	return arg1;
}

AsmArg* AsmCmd2::secondArg() const {
	return arg2;
}

bool AsmCmd2::changeStack() const {
	return *arg1 == EBP || *arg1 == ESP || *arg2 == EBP || *arg2 == ESP;
}

bool AsmCmd2::operationWith(AsmArg *a) const {
	AsmIndirectArg *indir = dynamic_cast<AsmIndirectArg*>(a);
	AsmRegister *r1 = dynamic_cast<AsmRegister*>(arg1);
	AsmRegister *r2 = dynamic_cast<AsmRegister*>(arg2);
	if(indir && (r1 || r2))
		return (r1 ? indir->usedRegister(r1->reg) : 0) || (r2 ? indir->usedRegister(r2->reg) : 0);
	else
		return *arg1 == a || *arg2 == a;
}

bool AsmCmd2::usedRegister(Registers r) const {
	return arg1->usedRegister(r) || arg2->usedRegister(r);
}

bool AsmCmd2::operateWith(AsmArg* a) const {
	AsmIndirectArg* ind = dynamic_cast<AsmIndirectArg*>(a);
	AsmRegister* reg1 = dynamic_cast<AsmRegister*>(arg1);
	AsmRegister* reg2 = dynamic_cast<AsmRegister*>(arg2);
	return ind && (reg1 || reg2) ?
		(reg1 ? ind->usedRegister(reg1->reg) : 0) || (reg2 ? ind->usedRegister(reg2->reg) : 0) : 
		*arg1 == a || *arg2 == a;
}

AsmIOCmd::AsmIOCmd(Values m, AsmMemory *f, AsmArg *a) : AsmCmd(cmdINVOKE), mode(m), format(f), arg(a) {}

string AsmIOCmd::generate() const {
	string common = get_str() + " crt_" + (mode == _printf ? "printf" : "scanf") + ", " + "addr " + format->generate();
	if (arg)
		return  common + ", " + arg->generate();
	else
		return common;	
}

bool AsmIOCmd::changeStack() const {
	return true;
}

AsmRegister* makeArg(Registers reg){
	return new AsmRegister(reg);
}

AsmImmediateArg* makeArg(int val){
	return new AsmImmediateArg(val);
}

AsmDup* makeArgDup(int count){
	return new AsmDup(count);
}

AsmMemory* makeArgMemory(const string &name, bool lv){
	return new AsmMemory(name, lv);
}

AsmIndirectArg* makeIndirectArg(Registers reg, int offset){
	return new AsmIndirectArg(reg, offset);
}

AsmArgLabel* makeLabel(const string &name){
	return new AsmArgLabel(name);
}

AsmStringArg* makeString(const string &name){
	string str(name);
	for(int i = 0; i < str.length() - 2; i++)
		if(str.substr(i, 2) == "\\n"){
			string s1(str.substr(0, i)), s2(str.substr(i + 2));
			str = s1 + "\", 0dh, 0ah";
			if (s2.length() > 1)
				str += ", \"" + s2;	
		}
	str += ", 0";
	return new AsmStringArg(str);
}

AsmFloat* makeFloat(float val){
	return new AsmFloat(val);
}

int AsmCode::size() const {
	return commands.size();
}

void AsmCode::fflush(ofstream  &out) const {
	for(int i = 0; i < commands.size(); i++)
		out << (dynamic_cast<AsmInstrLabel*>(commands[i]) ? "" : "\t") << commands[i]->generate() << endl;
}

void AsmCode::replace(int index, AsmCmd *cmd){
	delete commands[index];
	commands[index] = cmd;
}

void AsmCode::deleteRange(int from, int to){
	for(int i = from; i < to + 1; i++)
		delete commands[i];
	commands.erase(commands.begin() + from, commands.begin() + to + 1);
}

void AsmCode::insert(AsmCmd *cmd, int index){
	commands.insert(commands.begin() + index, cmd);
}

void AsmCode::move(int from, int to){
	AsmInstruction *t = commands[from];
	commands.erase(commands.begin() + from);
	commands.insert(commands.begin() + to, t);
}

AsmInstruction* AsmCode::operator [] (int index) const {
	return commands[index];
}

AsmCode& AsmCode::operator << (AsmCmd *command){
	commands.push_back(command);
	return *this;
}

AsmCode& AsmCode::add(Commands c){
	commands.push_back(new AsmCmd(c));
	return *this;
}

AsmCode& AsmCode::add(Commands c, AsmArg *arg){
	commands.push_back(new AsmCmd1(c, arg));
	return *this;
}

AsmCode& AsmCode::add(Commands c, AsmArg *arg1, AsmArg *arg2){
	commands.push_back(new AsmCmd2(c, arg1, arg2));
	return *this;
}

AsmCode& AsmCode::add(Commands c, Registers r){
	commands.push_back(new AsmCmd1(c, new AsmRegister(r)));
	return *this;
}

AsmCode& AsmCode::add(Commands c, Registers r1, Registers r2){
	commands.push_back(new AsmCmd2(c, new AsmRegister(r1), new AsmRegister(r2)));
	return *this;
}

AsmCode& AsmCode::add(Commands c, Registers r, int value){
	commands.push_back(new AsmCmd2(c, new AsmRegister(r), new AsmImmediateArg(value)));
	return *this;
}

AsmCode& AsmCode::add(AsmArgLabel *label){
	commands.push_back(new AsmInstrLabel(label));
	return *this;
}

AsmCode& AsmCode::add(Values val, AsmMemory *format, AsmArg *arg){
	commands.push_back(new AsmIOCmd(val, format, arg));
	return *this;
}

void CodeGenerator::generate(){
	ofstream out(file);
	if(!out)
		throw MyException("Cannot open file");
	out << ".686\n"
		   ".model flat, stdcall\n"
		   "include c:\\masm32\\include\\msvcrt.inc\n"
		   "includelib c:\\masm32\\lib\\msvcrt.lib\n"
		   ".data\n";
	data.fflush(out);
	out << ".code\n";
	code.fflush(out);
	out << "end start";
}