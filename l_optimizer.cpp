#include "l_optimizer.h"

bool AddSubESPZeroOptimization::optimize(AsmCode &code, int index){
	if (prepare(code[index]) && *cmd1->firstArg() == ESP && *cmd1->secondArg() == 0){
		code.deleteRange(index, index);
		return true;
	}
	return false;
}

bool PushPop2MovOptimization::optimize(AsmCode& code, int index){
	if(	prepare(code[index], code[index + 1]) && *cmd1 == cmdPUSH && *cmd2 == cmdPOP 
		&& !(cmd1->argument()->isMemory() && cmd2->argument()->isMemory())){
		AsmCmd* optCmd = new AsmCmd2(cmdMOV, cmd2->argument(), cmd1->argument());
		code.deleteRange(index, index + 1);
		code.insert(optCmd, index);	
		return true;
	} 
	return false;
}

bool PushPop2NilOptimization::optimize(AsmCode& code, int index){
	if(	prepare(code[index], code[index + 1]) && *cmd1 == cmdPUSH && *cmd2 == cmdPOP 
		&& cmd1->argument()->isRegister() && *cmd1->argument() == cmd2->argument())	{
			code.deleteRange(index, index + 1);
			return true;
	}
	return false;
}

bool MovChainOptimization::optimize(AsmCode& code, int index){
	if (prepare(code[index], code[index + 1]) 
		&& *cmd1 == cmdMOV && *cmd1->firstArg() == EAX 
		&& *cmd2 == cmdMOV && *cmd2->secondArg() == cmd1->firstArg()
		&& !(cmd1->secondArg()->isMemory() && cmd2->firstArg()->isMemory()) ){
			AsmCmd* optCmd = new AsmCmd2(cmdMOV, cmd2->firstArg(), cmd1->secondArg());
			code.deleteRange(index, index + 1);
			code.insert(optCmd, index);
			return true;
	}
	return false;
}

bool Neg2MovOppositeOptimization::optimize(AsmCode& code, int index){
	if (prepare(code[index], code[index + 1]) 
		&& *cmd1 == cmdMOV && *cmd1->firstArg() == EAX
		&& *cmd2 == cmdNEG && *cmd2->argument() == EAX
		&& dynamic_cast<AsmImmediateArg*>(cmd1->secondArg())){
			int val = dynamic_cast<AsmImmediateArg*>(cmd1->secondArg())->value;
			AsmCmd2* optCmd = new AsmCmd2(cmdMOV, makeArg(EAX), makeArg(-val));
			code.deleteRange(index, index + 1);
			code.insert(optCmd, index);
			return true;
	}
	return false;
}

bool Jmp2NextLineOptimization::optimize(AsmCode& code, int index){
	if (prepare(code[index], code[index + 1]) && *cmd1->argument() == cmd2->label){
		code.deleteRange(index, index);
		return true;
	}
	return false;	
}

bool MovCycle2NilOptimization::optimize(AsmCode& code, int index){
	if (prepare(code[index], code[index + 1]) 
		&& *cmd1 == cmdMOV && *cmd2 == cmdMOV
		&& *cmd1->firstArg() == EBX && *cmd2->firstArg() == EAX
		&& *cmd1->firstArg() == cmd2->secondArg()
		&& *cmd1->secondArg() == cmd2->firstArg()){
			code.deleteRange(index, index + 1);
			return true;
	}
	return false;
}

bool MovPush2PushOptimization::optimize(AsmCode& code, int index){
	if (prepare(code[index], code[index + 1]) 
		&& *cmd1 == cmdMOV && *cmd1->firstArg() == EAX
		&& *cmd2 == cmdPUSH && *cmd1->firstArg() == cmd2->argument()){
			AsmCmd1* optCmd = new AsmCmd1(cmdPUSH, cmd1->secondArg());
			code.deleteRange(index, index + 1);
			code.insert(optCmd, index);
			return true;
	}  
	return false;
}

bool RegRegCMP2RegIntCmpOptimization::optimize(AsmCode& code, int index){
	if(	prepare(code[index], code[index + 1]) 
		&& *cmd1 == cmdMOV && cmd2 && *cmd2 == cmdCMP
		&& *cmd1->firstArg() == cmd2->secondArg()){
			AsmCmd2* optCmd = new AsmCmd2(cmdCMP, cmd2->firstArg(), cmd1->secondArg());
			code.deleteRange(index, index + 1);
			code.insert(optCmd, index);
			return true;
	} 
	return false;
}

bool MultIntByInt2MovOptimization::optimize(AsmCode& code, int index){
	if(	prepare(code[index], code[index + 1], code[index + 2])
		&& *cmd1 == cmdMOV && *cmd2 == cmdMOV
		&& *cmd1->firstArg() == EAX && *cmd2->firstArg() == EBX
		&& cmd1->secondArg()->isImmediate() && cmd2->secondArg()->isImmediate()
		&& *cmd3 == cmdIMUL){
			int val1 = dynamic_cast<AsmImmediateArg*>(cmd1->secondArg())->value,
				val2 = dynamic_cast<AsmImmediateArg*>(cmd2->secondArg())->value;
			AsmCmd2* optCmd = new AsmCmd2(cmdMOV, cmd3->firstArg(), makeArg(val1 * val2));
			code.deleteRange(index, index + 2);
			code.insert(optCmd, index);
			return true;
	}
	return false;
}

bool AddZero2MovOptimization::optimize(AsmCode& code, int index){
	if( prepare(code[index], code[index + 1], code[index + 2])
		&& *cmd1 == cmdMOV && *cmd2 == cmdMOV && *cmd3 == cmdADD 
		&& (*cmd1->secondArg() == 0 || *cmd2->secondArg() == 0)){
			AsmCmd2* optCmd = new AsmCmd2(cmdMOV, cmd3->firstArg(), *cmd1->secondArg() == 0 ? cmd2->secondArg() : cmd1->secondArg());
			code.deleteRange(index, index + 2);
			code.insert(optCmd, index);
			return true;
	} 
	return false;
}

bool CompactAdditionOptimization::optimize(AsmCode& code, int index){
	if(	prepare(code[index], code[index + 1], code[index + 2])
		&& *cmd1 == cmdMOV && *cmd2 == cmdMOV && *cmd1->firstArg() == EBX 
		&& *cmd1->secondArg() == cmd2->firstArg() && *cmd3 == cmdADD 
		&& *cmd3->firstArg() == cmd2->firstArg() && *cmd3->secondArg() == cmd1->firstArg()){
			AsmCmd2* optCmd = new AsmCmd2(cmdMOV, makeArg(EBX), cmd2->secondArg());
			code.deleteRange(index, index + 1);
			code.insert(optCmd, index);
			return true;
	} 
	return false;
}

bool Mov2MemoryDirectlyOptimization::optimize(AsmCode& code, int index){
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd2* cmd2 = dynamic_cast<AsmCmd2*>(code[index + 1]);
	AsmCmd2* cmd3 = dynamic_cast<AsmCmd2*>(code[index + 2]);
	AsmCmd2* cmd4 = dynamic_cast<AsmCmd2*>(code[index + 3]);
	if (cmd1 && *cmd1->firstArg() == EAX && cmd1->secondArg()->isOffset()
		&& cmd2 && *cmd2->firstArg() == EBX && cmd2->secondArg()->isImmediate()
		&& cmd3 && cmd3->firstArg()->isMemory() && *cmd3->secondArg() == EBX
		&& cmd4 && *cmd4 == cmdMOV){
			cmd1->secondArg()->clearOffset();
			AsmCmd2* optCmd1 = new AsmCmd2(cmdMOV, cmd1->secondArg(), cmd2->secondArg());
			AsmCmd2* optCmd2 = new AsmCmd2(cmdMOV, cmd4->firstArg(), cmd2->secondArg());
			code.deleteRange(index, index + 3);
			code.insert(optCmd2, index);
			code.insert(optCmd1, index);
			return true;
	}	
	return false;
}

Optimizer::Optimizer() : oneOpOpts(0), twoOpOpts(0), threeOpOpts(0), fourOpOpts(0){
	oneOpOpts.push_back(new AddSubESPZeroOptimization());
	
	twoOpOpts.push_back(new PushPop2NilOptimization());
	twoOpOpts.push_back(new PushPop2MovOptimization());
	twoOpOpts.push_back(new MovChainOptimization());
	twoOpOpts.push_back(new Neg2MovOppositeOptimization());
	twoOpOpts.push_back(new Jmp2NextLineOptimization());	
	twoOpOpts.push_back(new MovPush2PushOptimization());
	twoOpOpts.push_back(new RegRegCMP2RegIntCmpOptimization());
	
	threeOpOpts.push_back(new AddZero2MovOptimization());
	threeOpOpts.push_back(new MultIntByInt2MovOptimization());
	threeOpOpts.push_back(new CompactAdditionOptimization());
	
	fourOpOpts.push_back(new Mov2MemoryDirectlyOptimization());
	
	postTwoOpOpts.push_back(new MovCycle2NilOptimization());
}

void Optimizer::pushDownPopUp(AsmCode& code){
	for (int i = 0; i < code.size(); i++){
		AsmCmd1* cmd = dynamic_cast<AsmCmd1*>(code[i]);
		int j = i;
		if (cmd && *cmd == cmdPUSH){			
			while (j + 1 < code.size() && !code[j + 1]->changeStack() && !code[j + 1]->operateWith(cmd->argument()))
				j++;
			code.move(i, j);
		} else if (cmd && *cmd == cmdPOP) {
			while (j - 1 > -1 && !code[j - 1]->changeStack() && !code[j - 1]->operateWith(cmd->argument()))
				j--;
			code.move(i, j);
		}
	}
}

void Optimizer::deleteUselessMovs(AsmCode& code){
	for (int i = 0; i < code.size(); i++){
		if (*code[i] != cmdMOV || !code[i]->usedRegister(EAX))
			continue;
		AsmCmd2* cmd = dynamic_cast<AsmCmd2*>(code[i]);
		if (cmd->secondArg()->usedRegister(EAX))
			continue;
		if (*cmd->firstArg() != EAX)
			continue;
		int idx = i + 1;
		bool deletingNedeed = true;
		while (idx < code.size() && deletingNedeed){
			if (code[idx]->usedRegister(EAX))
				if (*code[idx] != cmdMOV)
					deletingNedeed = false;
				else {
					AsmCmd2* tmp = dynamic_cast<AsmCmd2*>(code[idx]);
					if (tmp->secondArg()->usedRegister(EAX)	|| dynamic_cast<AsmIndirectArg*>(tmp->firstArg()))
						deletingNedeed = false;
					else
						break;
				}
			idx++;
		}
		if (deletingNedeed)
			code.deleteRange(i, i);
	}
}

void Optimizer::deleteUselessLabels(AsmCode& code){
	for (int i = 0; i < code.size(); i++){
		AsmInstrLabel* label = dynamic_cast<AsmInstrLabel*>(code[i]);
		if (!label || *label == string("start"))
			continue;
		bool unused = true;
		for (int j = 0; j < code.size() && unused; j++){
			if (j == i)
				continue;
			if (code[j]->isJump()){
				AsmArgLabel* dstn = dynamic_cast<AsmArgLabel*>(dynamic_cast<AsmCmd1*>(code[j])->argument());
				unused = *label->label != dstn;
			}
		}
		if (unused)
			code.deleteRange(i, i);
	}
}

void Optimizer::optimize(AsmCode& code){
	while(true)	{
		pushDownPopUp(code);
		bool goToNextIteration = false;
		for (int i = 0; i < code.size(); i++)
			for (int j = 0; j < oneOpOpts.size(); j++)
				if (oneOpOpts[j]->optimize(code, i))
					goToNextIteration = true;				
		if(goToNextIteration)
			continue;
		for(int i = 0; i < code.size() - 1; i++)
			for (int j = 0; j < twoOpOpts.size(); j++)
				if (twoOpOpts[j]->optimize(code, i))
					goToNextIteration = true;
		if(goToNextIteration)
			continue;
		for(int i = 0; i < code.size() - 2; i++)
			for(int j = 0; j < threeOpOpts.size(); j++)
				if (threeOpOpts[j]->optimize(code, i))
					goToNextIteration = true;
		for(int i = 0; i < code.size() - 3; i++)
			for (int j = 0; j < fourOpOpts.size(); j++)
				if (fourOpOpts[j]->optimize(code, i))
					goToNextIteration = true;
		if(!goToNextIteration)
			break;
	}
	deleteUselessMovs(code);
	deleteUselessLabels(code);
	while(true){
		bool goToNextIteration = false;
		for (int i = 0; i < code.size() - 1; i++)
			for (int j = 0; j < postTwoOpOpts.size(); j++)
				if (postTwoOpOpts[j]->optimize(code, i))
					goToNextIteration = true;
		if (goToNextIteration)
			continue;
		else
			break;
	}
}