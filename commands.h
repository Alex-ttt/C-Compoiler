#pragma once

#include "scan.h"
#include <vector>

enum Commands{
	cmdMOV,
	cmdPUSH,
	cmdPOP,
	cmdMUL,
	cmdIMUL,
	cmdDIV,
	cmdIDIV,
	cmdADD,
	cmdSUB,
	cmdINC,
	cmdDEC,
	cmdRET,
	cmdNEG,
	cmdCDQ,
	cmdINVOKE,
	cmdSHR,
	cmdSHL,
	cmdAND,
	cmdOR,
	cmdXOR,
	cmdNOT,
	cmdCALL,
	cmdJMP,
	cmdCMP,
	cmdJE,
	cmdJNE,
	cmdSETG,
	cmdSETL,
	cmdSETGE,
	cmdSETLE,
	cmdSETE,
	cmdSETNE,
	cmdFDIVP,
	cmdFADDP,
	cmdFMULP,
	cmdFSUBP,
	cmdFLD,
	cmdFSTP,
	cmdFCHS,
	cmdFILD,
	cmdFISTP,
	cmdFLD1,
	cmdFCOMPP,
	cmdFNSTSW,
	cmdSAHF,
	cmdREAL4,
	cmdREAL8,
	cmdDD,
	cmdDB,
	cmdDQ
};

enum Registers{
	EAX,
	EBX,
	ECX,
	EDX,
	EBP,
	ESP,
	CL,
	AL,
	BL,
	AX
};


class AsmArg{
public:
	virtual string generate() const = 0;
	virtual bool operator == (int v) const { return false; }
	virtual bool isImmediate() const { return false; }
	virtual bool operator == (AsmArg *a) const { return false; }
	virtual bool operator != (AsmArg *a) const { return !(*this == a); }
	virtual bool operator == (Registers r) const { return false; }
	virtual bool operator != (Registers r) const { return !(*this == r); }
	virtual bool isRegister() const { return false; }
	virtual bool isMemory() const { return false; }
	virtual bool isOffset() const { return false; }
	virtual bool usedRegister(Registers r) const { return false; }
	virtual void clearOffset() {}
};


class AsmImmediateArg : public AsmArg{
public:
	int value;
	AsmImmediateArg(int v) : value(v) {}
	string generate() const;
	bool operator == (int v) const;
	bool isImmediate() const;

};

class AsmStringArg : public AsmArg{
private:
	string value;

public:
	AsmStringArg(const string &str) : value(str) {}
	string generate() const;

};

class AsmRegister : public AsmArg{
protected:
	string get_str() const;

public:
	Registers reg;
	AsmRegister(Registers r) : reg(r) {}
	string generate() const;
	bool operator == (AsmArg *a) const;
	bool usedRegister(Registers reg) const;
	virtual bool operator == (Registers r) const;
	bool isRegister() const;
};

class AsmIndirectArg : public AsmRegister{
private:
	int offset;

public:
	AsmIndirectArg(Registers r, int of = 0) : AsmRegister(r), offset(of) {}
	string generate() const;
	bool operator == (AsmArg *a) const;
	bool operator == (Registers r) const;
	bool usedRegister(Registers r) const;
	bool isMemory() const;
};

class AsmMemory : public AsmArg{
private:
	string varName;
	bool lvalue;

public:
	AsmMemory(const string &str, bool l = false) : varName(str), lvalue(l) {}
	string generate() const;
	bool operator == (AsmArg *a) const;
	bool isMemory() const;
	bool isOffset() const;
	void clearOffset();
};

class AsmArgLabel : public AsmArg{
private:
	string labelName;

public:
	friend class AsmInstrLabel;
	AsmArgLabel(const string &str) : labelName(str) {}
	string generate() const;
	string name() const;
	bool operator == (AsmArg *a) const;
	friend class GoToNode;
	friend class Block;
};

class AsmDup : public AsmArg{
private:
	int count;

public:
	AsmDup(int c) : count(c) {}
	string generate() const;
};

class AsmFloat : public AsmArg{
private:
	float value;

public:
	AsmFloat(float v) : value(v) {}
	string generate() const;
};

class AsmInstruction{
public:
	virtual string generate() const = 0;
	virtual bool operator == (Commands c) const { return false; }
	virtual bool usedRegister(Registers r) const { return false; }
	virtual bool changeStack() const { return false; }
	virtual bool isJump() const { return false; }
	virtual bool operateWith(AsmArg* arg) const { return false; }
	virtual bool operationWith(AsmArg *a) const { return false; }
	bool operator != (Commands c) const { return !(*this == c); }
};

class AsmInstrLabel : public AsmInstruction{
public:
	AsmArgLabel *label;
	AsmInstrLabel(AsmArgLabel *l) : label(l) {}
	bool operator == (const string &str) const;
	virtual string generate() const;
};

class AsmCmd : public AsmInstruction{
protected:
	Commands operation;
	string get_str() const;

public:
	AsmCmd(){}
	AsmCmd(Commands c) : operation(c) {}
	virtual string generate() const;
	bool operator == (Commands c) const;
};

class AsmCmd1 : public AsmCmd{
private:
	AsmArg *arg;

public:
	AsmCmd1(Commands c, AsmArg *a) : AsmCmd(c), arg(a) {}
	string generate() const;
	AsmArg* argument() const;
	bool changeStack() const;
	bool operationWith(AsmArg *a) const;
	bool usedRegister(Registers r) const;
	bool isJump() const;
	bool operateWith(AsmArg* a) const;
};

class AsmCmd2 : public AsmCmd{
private:
	AsmArg *arg1;
	AsmArg *arg2;

public:
	AsmCmd2(Commands c, AsmArg *a1, AsmArg *a2) : AsmCmd(c), arg1(a1), arg2(a2) {}
	string generate() const;
	AsmArg *firstArg() const;
	AsmArg *secondArg() const;
	bool changeStack() const;
	bool operationWith(AsmArg *a) const;
	bool usedRegister(Registers r) const;
	bool operateWith(AsmArg* a) const;
};

class AsmIOCmd : public AsmCmd{
private:
	Values mode;
	AsmMemory *format;
	AsmArg *arg;

public:
	AsmIOCmd(Values m, AsmMemory *f, AsmArg *a);
	string generate() const;
	bool changeStack() const;
};

AsmRegister* makeArg(Registers reg);
AsmImmediateArg* makeArg(int val);
AsmDup* makeArgDup(int count);
AsmMemory* makeArgMemory(const string &name, bool lv = false);
AsmIndirectArg* makeIndirectArg(Registers reg, int offset = 0);
AsmArgLabel* makeLabel(const string &name);
AsmStringArg* makeString(const string &name);
AsmFloat* makeFloat(float val);

class AsmCode{
private:
	vector<AsmInstruction*> commands;

public:
	AsmCode() : commands(0) {}
	int size() const;
	void fflush(ofstream  &out) const;
	void replace(int index, AsmCmd *cmd);
	void deleteRange(int from, int to);
	void insert(AsmCmd *cmd, int index);
	void move(int from,  int to);
	AsmInstruction* operator [] (int index) const;
	AsmCode& operator << (AsmCmd *command);
	AsmCode& add(Commands c);
	AsmCode& add(Commands c, AsmArg *arg);
	AsmCode& add(Commands c, AsmArg *arg1, AsmArg *arg2);
	AsmCode& add(Commands c, Registers r);
	AsmCode& add(Commands c, Registers r1, Registers r2);
	AsmCode& add(Commands c, Registers r, int value);
	AsmCode& add(AsmArgLabel *label);
	AsmCode& add(Values val, AsmMemory *format, AsmArg *arg = 0); 
};