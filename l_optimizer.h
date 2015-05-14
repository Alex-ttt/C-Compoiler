#pragma once

#include "commands.h"
#include <vector>

using namespace std;

class Optimization{
public:
	virtual bool optimize(AsmCode &code, int index) = 0;
};

template <class T1>
class OneOperationOptimization : public Optimization{
protected:
	T1* cmd1;
public:
	OneOperationOptimization(): cmd1(0) {}
	bool prepare(AsmInstruction* ins){
		cmd1 = dynamic_cast<T1*>(ins);
		return cmd1 != 0;
	}
};

template <class T1, class T2>
class TwoOperationOptimization : public OneOperationOptimization<T1>{
protected:
	T2* cmd2;
public:
	TwoOperationOptimization(): cmd2(0) {}
	bool prepare(AsmInstruction* ins1, AsmInstruction* ins2){
		cmd2 = dynamic_cast<T2*>(ins2);
		return OneOperationOptimization<T1>::prepare(ins1) && cmd2 != 0;
	}
};

template <class T1, class T2, class T3>
class ThreeOperationOptimization : public TwoOperationOptimization<T1, T2>{
protected:
	T3* cmd3;
public:
	ThreeOperationOptimization(): cmd3(0) {}
	bool prepare(AsmInstruction* ins1, AsmInstruction* ins2, AsmInstruction* ins3){
		cmd3 = dynamic_cast<T3*>(ins3);
		return TwoOperationOptimization<T1, T2>::prepare(ins1, ins2) && cmd3 != 0;
	}
};

class FourOperationOptimization : public Optimization {};

class AddSubESPZeroOptimization : public OneOperationOptimization<AsmCmd2>{
public:
	bool optimize(AsmCode &code, int index);
};

typedef TwoOperationOptimization<AsmCmd1, AsmCmd1> C1C1Opt;
typedef TwoOperationOptimization<AsmCmd2, AsmCmd2> C2C2Opt;
typedef TwoOperationOptimization<AsmCmd1, AsmCmd2> C1C2Opt;
typedef TwoOperationOptimization<AsmCmd2, AsmCmd1> C2C1Opt;

class PushPop2MovOptimization : public C1C1Opt{
public:
	bool optimize(AsmCode& code, int index);
};

class PushPop2NilOptimization : public C1C1Opt{
public:
	bool optimize(AsmCode& code, int index);
};

class MovChainOptimization : public C2C2Opt{
public:
	bool optimize(AsmCode& code, int index);
};

class Neg2MovOppositeOptimization : public C2C1Opt{
public:
	bool optimize(AsmCode& code, int index);
};

class Jmp2NextLineOptimization : public TwoOperationOptimization<AsmCmd1, AsmInstrLabel>{
public:
	bool optimize(AsmCode& code, int index);
};

class MovCycle2NilOptimization : public C2C2Opt{
public:
	bool optimize(AsmCode& code, int index);
};

class MovPush2PushOptimization : public C2C1Opt{
public:
	bool optimize(AsmCode& code, int index);
};

class RegRegCMP2RegIntCmpOptimization : public C2C2Opt{
public:
	bool optimize(AsmCode& code, int index);
};

typedef ThreeOperationOptimization<AsmCmd2, AsmCmd2, AsmCmd2> C2C2C2Opt;

class MultIntByInt2MovOptimization : public C2C2C2Opt{
public:
	bool optimize(AsmCode& code, int index);
};

class AddZero2MovOptimization : public C2C2C2Opt{
public:
	bool optimize(AsmCode& code, int index);
};

class CompactAdditionOptimization : public C2C2C2Opt{
public:
	bool optimize(AsmCode& code, int index);
};

class Mov2MemoryDirectlyOptimization : public FourOperationOptimization{
public:
	bool optimize(AsmCode& code, int index);
};

class Optimizer
{
private:
	vector<Optimization*> oneOpOpts;
	vector<Optimization*> twoOpOpts;
	vector<Optimization*> threeOpOpts;
	vector<Optimization*> fourOpOpts;
	vector<Optimization*> postTwoOpOpts;
	void pushDownPopUp(AsmCode& code);
	void deleteUselessMovs(AsmCode& code);
	void deleteUselessLabels(AsmCode& code);
public:
	Optimizer();
	void optimize(AsmCode& code);
};