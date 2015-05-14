#include "nodes.h"

void Node::generateST0ToStack(AsmCode &code){
	code.add(cmdFSTP, real4)
		.add(cmdPUSH, real4);
}

void Node::generateByteToFPU(AsmCode &code){
	code.add(cmdPOP, EAX)
		.add(cmdMOV, real4, makeArg(EAX))
		.add(cmdFLD, real4);
}

void BinaryOpNode::generateForFloat(AsmCode& code){
	left->generateFPUStack(code);
	right->generateFPUStack(code);
	Commands cmd;
	Values val = token->value;
	if(isComparison(val)){
		code.add(cmdFCOMPP)
			.add(cmdFNSTSW, AX)
			.add(cmdSAHF)
			.add(cmdMOV, EAX, 0);
		switch(val){
		case more:
			cmd = cmdSETG;
			break;
		case more_or_eql:
			cmd = cmdSETGE;
		case Values::less:
			cmd = cmdSETL;
			break;
		case less_or_eql:
			cmd = cmdSETLE;
			break;
		case equally:
			cmd =cmdSETE;
			break;
		case not_eql:
			cmd = cmdSETNE;
			break;
		}
		code.add(cmd, AL)
			.add(cmdPUSH, EAX);
	} else {
		switch(val){
		case plus:
		case pl_eql:
			cmd = cmdFADDP;
			break;
		case minus:
		case mn_eql:
			cmd = cmdFSUBP;
			break;
		case divis:
		case div_eql:
			cmd = cmdFDIVP;
			break;
		case factor:
		case fct_eql:
			cmd = cmdFMULP;
			break;
		default:
			throw MyException("Compiler error", token);
			break;
		}
		code.add(cmd);
		generateST0ToStack(code);
		if(isAssignment(val)){
			left->generateLvalue(code);
			code.add(cmdPOP, EAX)
				.add(cmdPOP, EBX)
				.add(cmdMOV, makeIndirectArg(EAX), makeArg(EBX))
				.add(cmdPUSH, EBX);
		}
	}
}

void BinaryOpNode::generateFPUStack(AsmCode& code){
	generate(code);
	generateByteToFPU(code);
}

void BinaryOpNode::generateLvalue(AsmCode& code){
	Values value = token->value;
	if(value == point || value == arrow){
		if(value == point)
			left->generateLvalue(code);
		else
			left->generate(code);
		code.add(cmdPOP, EAX)
			.add(cmdMOV, EBX, dynamic_cast<IdentifierNode*>(right)->var->offset)
			.add(cmdADD, EAX, EBX)
			.add(cmdPUSH, EAX);
	} else if(isAssignment(value)){ // (a = b) = c
		generate(code);
		code.add(cmdPOP, EAX);
		left->generateLvalue(code);
	} else
		throw MyException("Compiler error");
}

void BinaryOpNode::generate(AsmCode &code){
	Values op = token->value;
	SymbolType *leftType = left->nodeType ? left->nodeType : left->getType();
	SymbolType *rightType = right->nodeType ? right->nodeType : right->getType();
	PointerSymbol *leftPointer = dynamic_cast<PointerSymbol*>(leftType);
	PointerSymbol *rightPointer = dynamic_cast<PointerSymbol*>(rightType);	
	if (!leftPointer && dynamic_cast<ArraySymbol*>(leftType))
		leftPointer = dynamic_cast<ArraySymbol*>(leftType)->convertToPointer();
	if (!rightPointer && dynamic_cast<ArraySymbol*>(rightType))
		rightPointer = dynamic_cast<ArraySymbol*>(rightType)->convertToPointer();
	if((op == plus || op == Values::minus) && (leftPointer || rightPointer) && !(leftPointer && rightPointer)){
		if(!leftPointer){
			swap(left, right);
			swap(leftPointer, rightPointer);
		}
		left->generateLvalue(code);
		right->generate(code);
		code.add(cmdPOP, EAX)
			.add(cmdMOV, EBX, leftPointer->pointer->byteSize())
			.add(cmdIMUL, EBX, EAX)
			.add(cmdPOP, EAX)
			.add(op == Values::plus ? cmdADD : cmdSUB, EAX, EBX)
			.add(cmdPUSH, EAX);
		return;
	}
	if(op == Values::minus && leftPointer && rightPointer){
		left->generate(code);
		right->generate(code);
		code.add(cmdPOP, EAX)
			.add(cmdPOP, EBX)
			.add(cmdSUB, EAX, EBX)
			.add(cmdMOV, EBX, leftPointer->pointer->byteSize())
			.add(cmdCDQ)
			.add(cmdIDIV, EBX)
			.add(cmdPUSH, EAX);
		return;
	}
	if(op == point || op == arrow){
		generateLvalue(code);
		code.add(cmdPOP, EAX)
			.add(cmdPUSH, makeIndirectArg(EAX));
	} else	{
		right->generate(code);
		if(op == assign){
			left->generateLvalue(code);
			code.add(cmdPOP, EAX);
			int size = right->getType()->byteSize();
			int n = size / 4 + (size % 4 != 0);
			for(int i = 0; i < n; i++)
				code.add(cmdPOP, EBX)
					.add(cmdMOV, makeIndirectArg(EAX, i * 4), makeArg(EBX));
			code.add(cmdMOV, EAX, EBX);
		} else {
			if(*getType() == FloatType){
				generateForFloat(code);
				return;
			}
			AsmArg *l, *r;
			if(isAssignment(op)){
				left->generateLvalue(code);
				l = makeIndirectArg(EAX);
				r = makeArg(EBX);
				code.add(cmdPOP, EAX)
					.add(cmdPOP, EBX);
			} else {
				l = makeArg(EAX);
				r = makeArg(EBX);
				left->generate(code);
				code.add(cmdPOP, EAX)
					.add(cmdPOP, EBX);
			}
			if(op == comma)
				code.add(cmdMOV, EAX, EBX);
			else if(op == divis || op == percent){
				code.add(cmdCDQ).add(cmdIDIV, r);
				if(op == percent)
					code.add(cmdMOV, l, makeArg(EDX));
			} else if(op == div_eql || op == per_eql){
				code.add(cmdMOV, ECX, EAX)
					.add(cmdMOV, makeArg(EAX), makeIndirectArg(ECX))
					.add(cmdCDQ)
					.add(cmdIDIV, r)
					.add(cmdMOV, makeIndirectArg(ECX), makeArg(op == per_eql ? EDX : EAX));
				if(op == per_eql)
					code.add(cmdMOV, EAX, EDX);
			} else if(op == fct_eql)
				code.add(cmdIMUL, r, l)
					.add(cmdMOV, l, r)
					.add(cmdMOV, makeArg(EAX), r);
			else if(op ==sl_eql || op == sr_eql || op == sl || op == sr){
				code.add(cmdMOV, makeArg(ECX), r)
					.add(op == sl || op == sl_eql ? cmdSHL : cmdSHR, l, makeArg(CL));
			} else if(isComparison(op)){
				code.add(cmdCMP, l, r).add(cmdMOV, EAX, 0);
				Commands cmd;
				switch(op){
				case Values::less:
					cmd = cmdSETL;
					break;
				case less_or_eql:
					cmd = cmdSETLE;
					break;
				case more:
					cmd = cmdSETG;
					break;
				case more_or_eql:
					cmd = cmdSETGE;
					break;
				case equally:
					cmd = cmdSETE;
					break;
				case l_not:
					cmd = cmdSETNE;
					break;
				}
				code.add(cmd, AL);
			} else if(op == l_and || op == l_or)
				code.add(cmdCMP, EAX, 0)
					.add(cmdMOV, EAX, 0)
					.add(cmdSETNE, AL) 
					.add(cmdCMP, EBX, 0)
					.add(cmdMOV, EBX, 0)
					.add(cmdSETNE, BL)
					.add(op == l_and ? cmdIMUL : cmdADD, EAX, EBX)
					.add(cmdCMP, EAX, 0)
					.add(cmdMOV, EAX, 0)
					.add(cmdSETNE, AL);
			else {
				Commands cmd;
				switch(op){
				case plus:
				case pl_eql:
					cmd = cmdADD;
					break;
				case minus:
				case mn_eql:
					cmd = cmdSUB;
					break;
				case factor:
					cmd = cmdIMUL;
					break;
				case b_and:
				case and_eql:
					cmd = cmdAND;
					break;
				case b_or:
				case or_eql:
					cmd = cmdOR;
					break;
				case xor:
				case xor_eql:
					cmd = cmdXOR;
					break;
				}
				code.add(cmd, l, r);
				if(isAssignment(op))
					code.add(cmdMOV, ECX, EAX)
						.add(cmdMOV, makeArg(EAX), makeIndirectArg(ECX));
			}
		}
		code.add(cmdPUSH, EAX);
	}
}

void GoToBinaryOpNode::generate(AsmCode &code){
	BinaryOpNode::generate(code);
	code.add(cmdJMP, label);
}

void UnaryOpNode::generateLvalue(AsmCode& code){
	if(token->value == factor)
		operand->generate(code);
	else 
		throw MyException("Compiler error");
}

void UnaryOpNode::generateFPUStack(AsmCode& code){
	generate(code);
	generateByteToFPU(code);
}

void UnaryOpNode::generate(AsmCode &code){
	Values op = token->value;
	SymbolType *type = operand->nodeType ? operand->nodeType : operand->getType();
	if(op == b_and)
		operand->generateLvalue(code);
	else 
		if(op == incr || op == decr){
			operand->generate(code);
			if((operand->nodeType ? operand->nodeType : operand->getType()) == FloatType){
				generateByteToFPU(code);
				code.add(cmdFLD1)
					.add(op == incr ? cmdFADDP : cmdFSUBP);
				generateST0ToStack(code);
			}
			else {
				code.add(cmdPOP, EAX);
				PointerSymbol *pointer = dynamic_cast<PointerSymbol*>(type);
				if(pointer)
					code.add(cmdMOV, EBX, pointer->pointer->byteSize())
					.add(op == incr ? cmdADD : cmdSUB, EAX, EBX);
				else
					code.add(op == incr ? cmdINC : cmdDEC, EAX);
				code.add(cmdPUSH, EAX);
			}
			operand->generateLvalue(code);
			code.add(cmdPOP, EBX)
				.add(cmdPOP, EAX)
				.add(cmdMOV, makeIndirectArg(EBX), makeArg(EAX))
				.add(cmdPUSH, EAX);
	} else
		if(op == l_not){
			operand->generate(code);
			code.add(cmdPOP, EAX)
				.add(cmdCMP, EAX, 0)
				.add(cmdSETE, AL)
				.add(cmdPUSH, EAX);
		}
		else {
			operand->generate(code);
			code.add(cmdPOP, EAX);
			if(op == Values::minus){
				if(type == FloatType){
					code.add(cmdPUSH, EAX);
					generateByteToFPU(code);
					code.add(cmdFCHS);
					generateST0ToStack(code);
				} else
					code.add(cmdNEG, EAX)
						.add(cmdPUSH, EAX);
			} else
				if(op == b_not)
					code.add(cmdNOT, EAX)
					.add(cmdPUSH, EAX);	
				else
					if(op == factor){
						int size = type->upType()->byteSize();
						int n = size / 4 + (size % 4 != 0);
						code.add(cmdMOV, EBX,EAX);
						for(int i = 0; i < n; i++)
							code.add(cmdMOV, makeArg(EAX), makeIndirectArg(EBX, (n - i - 1) * 4))
								.add(cmdPUSH, EAX);
					}
		}
}

void PostfixUnaryOpNode::generate(AsmCode &code){}

void IntNode::generate(AsmCode &code){
	code.add(cmdPUSH, makeArg(dynamic_cast<IntNumber*>(token)->v2));
}

void FloatNode::generateFPUStack(AsmCode& code){
	code.add(cmdFLD, makeArgMemory(constName()));
}

void FloatNode::GenerateData(AsmCode &code) const {
	code.add(cmdDD, makeArgMemory(constName()), makeFloat(dynamic_cast<FloatNumber*>(token)->v2));	
}

void IdentifierNode::generateFPUStack(AsmCode& code){
	if(var->global)
		code.add(cmdFLD, makeArgMemory("var_" + var->name));
	else
		code.add(cmdMOV, makeArg(EAX), makeIndirectArg(EBP, var->offset))
			.add(cmdMOV, real4, makeArg(EAX))
			.add(cmdFLD, real4);
}

void ArrNode::generate(AsmCode &code){
	generateLvalue(code);
	code.add(cmdPOP, EAX)
		.add(cmdPUSH, makeIndirectArg(EAX));

}

void ArrNode::generateLvalue(AsmCode &code){
	SymbolType *type = name->nodeType ? name->nodeType : name->getType();
	if(type->isArraySymbol())
		name->generateLvalue(code);
	else if(type->isPointerSymbol())
		name->generate(code);
	SymbolType *t = type->upType();
	for(int i = 0; i < args.size(); i++){
		args[i]->generate(code);
		code.add(cmdPOP, EAX)
			.add(cmdMOV, EBX, t->byteSize())
			.add(cmdIMUL, EAX, EBX)
			.add(cmdPOP, EBX)
			.add(cmdADD, EAX, EBX)
			.add(cmdPUSH, EAX);
		t = t->upType();
	}	
}

void StringNode::GenerateData(AsmCode &code) const {
	code.add(cmdDB, makeArgMemory("str" + to_string(index)), makeString("\"" + token->str + "\""));
}


void FunctionalNode::generateFPUStack(AsmCode& code){
	generate(code);
	generateByteToFPU(code);
}

void FuncCallNode::generate(AsmCode& code){
	code.add(cmdSUB, ESP, symbol->value->byteSize());	
	for (int i = args.size() - 1; i > -1; i--)
		args[i]->generate(code);
	code.add(cmdCALL, makeLabel("f_" + name->token->str))
		.add(cmdADD, ESP, symbol->params->byteSize());
}

void IONode::generate(AsmCode& code){
	int size = 0;
	for (int i = args.size() - 1; i > -1; i--){
		SymbolType *type = args[i]->getType();
		args[i]->generate(code);
		size += type->byteSize();
		if (*type == FloatType){
			code.add(cmdPOP, real4)
				.add(cmdFLD, real4)
				.add(cmdFSTP, real8)
				.add(cmdMOV, makeArg(EAX), makeArgMemory(real8name, true))
				.add(cmdPUSH, makeIndirectArg(EAX, 4))
				.add(cmdPUSH, makeIndirectArg(EAX));
			size += 4;
		}
	}
	code.add(token->value, makeArgMemory("str" + to_string(format->index)));
	code.add(cmdADD, ESP, size);
}

void FuncSymbol::generate(AsmCode &code, const string &str) const {
	code.add(makeLabel("f_" + str))
		.add(cmdPUSH, EBP)
		.add(cmdMOV, EBP, ESP);
	body->generate(code);
	code.add(endLabel)
		.add(cmdMOV, ESP, EBP)
		.add(cmdPOP, EBP)
		.add(cmdRET, makeArg(0));
}

void CastNode::generateFPUStack(AsmCode& code){
	generate(code);
	generateByteToFPU(code);	
}

void IfStatement::generate(AsmCode &code){
	string key = to_string(labelCounter++);
	AsmArgLabel *trueLabel = makeLabel("if" + key + "_true"),
		*falseLabel = makeLabel("if" + key + "_false"),
		*endLabel = makeLabel("if" + key + "_end");
	condition->generate(code);
	code.add(cmdPOP, EAX)
		.add(cmdCMP, EAX, 0)
		.add(cmdJNE, trueLabel)
		.add(cmdJE, falseLabel)
		.add(trueLabel);
	if_branch->generate(code);
	code.add(cmdJMP, endLabel).
		add(falseLabel);
	if(else_branch)
		else_branch->generate(code);
	code.add(endLabel);
}

void ForStatement::generate(AsmCode &code){
	string key = to_string(labelCounter++);
	startLabel = makeLabel("for" + key + "_start");
	endLabel = makeLabel("for" + key + "_end");
	third_cond_label = makeLabel("for" + key + "_incr");
	AsmArgLabel* conditionLabel = makeLabel("for" + key + "_condition");
	if(first_cond)
		first_cond->generate(code);
	code.add(startLabel)
		.add(conditionLabel);
	if(second_cond)
		second_cond->generate(code);
	code.add(cmdPOP, EAX)
		.add(cmdCMP, EAX, 0)
		.add(cmdJE, endLabel);
	body->generate(code);
	code.add(third_cond_label);
	if(third_cond)
		third_cond->generate(code);
	code.add(cmdJMP, conditionLabel)
		.add(endLabel);
}

void WhileStatement::generate(AsmCode &code){
	string key = to_string(labelCounter++);
	startLabel = makeLabel("while" + key + "_start");
	endLabel = makeLabel("while" + key + "_end");
	AsmArgLabel* cond = makeLabel("while" + key + "_condition");
	code.add(startLabel).
		add(cond);
	condition->generate(code);
	code.add(cmdPOP, EAX)
		.add(cmdCMP, EAX, 0)
		.add(cmdJE, endLabel);
	body->generate(code);
	code.add(cmdJMP, cond)
		.add(endLabel);
}

void DoWhileStatement::generate(AsmCode &code){
	string key = to_string(labelCounter++);
	startLabel = makeLabel("dowhile" + key + "_start");
	endLabel = makeLabel("dowhile" + key + "_end");
	code.add(startLabel);
	body->generate(code);
	condition->generate(code);
	code.add(cmdPOP, EAX)
		.add(cmdCMP, EAX, 0)
		.add(cmdJNE, startLabel)
		.add(endLabel);
}

void ContinueStatement::generate(AsmCode &code){
	ForStatement *For = dynamic_cast<ForStatement*>(owner);
	if(For)
		code.add(cmdJMP, For->third_cond_label);
	else
		code.add(cmdJMP, owner->startLabel);
}

void BreakStatement::generate(AsmCode &code){
	code.add(cmdJMP, owner->endLabel);
}

void ReturnStatement::generate(AsmCode &code){
	if(value){
		value->generate(code);
		int size = owner->value->byteSize();
		int steps = size / 4 + (size % 4 != 0);
		for (int i = 0; i < steps; i++)
			code.add(cmdPOP, EAX)
				.add(cmdMOV, makeIndirectArg(EBP, 4 + owner->params->byteSize() + size - 4 * (steps - i - 1)), makeArg(EAX));
	}
	code.add(cmdJMP, owner->endLabel);
}

void Block::generate(AsmCode &code){
	code.add(cmdSUB, ESP, table->byteSize());
	for(int i = 0; i < size(); i++){
		body[i]->generate(code);
		if(body[i]->isExpression()){
			SymbolType *type = body[i]->getType();
			if(type && type->byteSize())
				code.add(cmdPOP, EAX);
		}
	}
	code.add(endLabel);
}

void GoToNode::generate(AsmCode &code) const{
	code.add(cmdJMP, block->endLabel);
}