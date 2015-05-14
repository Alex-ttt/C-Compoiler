#include "h_otimizer.h"

InlineCall::InlineCall() : inlineCounter(0), insertIndex(0), blockShift(0) {}

int InlineCall::ArgInCallIndex(SymTable *table, IdentifierNode *iNode){
	VarSymbol *var = iNode->var;
	for(int i = 0; i < table->size(); i++)
		if(table->sym_ptr[i] == var)
			return i;
	return -1;
}

void InlineCall::HandleBlock(Block *block, SymTable *formalParams, VarSymbol *returnVar, AsmArgLabel *label){
	for(int i = 0; i < block->body.size(); i++){
		Node *expr = block->body[i];
		if(expr->isExpression())
			HandleInlineNodeOccur(expr, formalParams, returnVar);
		if(ReturnStatement *ret = dynamic_cast<ReturnStatement*>(expr)){
			if(ret->value){
				HandleInlineNodeOccur(ret->value, formalParams, returnVar);
				auto val = ret->value;
				delete expr;
				block->body[i] = new GoToBinaryOpNode(new Operation(assign), new IdentifierNode(returnVar), val, label);
			} else {
				delete ret;
				block->body[i] = new GoToNode(label);
			}
			return;
		}
		if(IfStatement *_if = dynamic_cast<IfStatement*>(expr)){
			HandleInlineNodeOccur(_if->condition, formalParams, returnVar);
			HandleBlock(dynamic_cast<Block*>(_if->if_branch), formalParams, returnVar, label);
			if(_if->else_branch)
				HandleBlock(dynamic_cast<Block*>(_if->else_branch), formalParams, returnVar, label);
			continue;
		}
		if(CycleStatement *loop = dynamic_cast<CycleStatement*>(expr)){
			HandleBlock(dynamic_cast<Block*>(loop->body), formalParams, returnVar, label);
			if(ForStatement *_for = dynamic_cast<ForStatement*>(loop)){
				HandleInlineNodeOccur(_for->first_cond, formalParams, returnVar);
				HandleInlineNodeOccur(_for->second_cond, formalParams, returnVar);
				HandleInlineNodeOccur(_for->third_cond, formalParams, returnVar);
			}
			if(WhileStatement *_while = dynamic_cast<WhileStatement*>(loop))
				HandleInlineNodeOccur(_while->condition, formalParams, returnVar);			
			if(DoWhileStatement *_dowhile = dynamic_cast<DoWhileStatement*>(loop))
				HandleInlineNodeOccur(_dowhile->condition, formalParams, returnVar);	
		}

	}
}

void InlineCall::HandleInlineNodeOccur(Node *&expr, SymTable *formalParams, VarSymbol *returnVar){

	if(IdentifierNode *iNode = dynamic_cast<IdentifierNode*>(expr)){
		int index = ArgInCallIndex(formalParams, iNode);
		if(index != -1){
			delete expr;
			expr = formalParams->callParams[index];
		}
		return;
	}
	if(BinaryOpNode *bNode = dynamic_cast<BinaryOpNode*>(expr)){
		HandleInlineNodeOccur(bNode->left, formalParams, returnVar);
		HandleInlineNodeOccur(bNode->right, formalParams, returnVar);
		if(bNode->isTernaryOpNode())
			HandleInlineNodeOccur(dynamic_cast<TernaryOpNode*>(expr)->condition, formalParams, returnVar);
		return;
	}
	if(UnaryOpNode *uNode = dynamic_cast<UnaryOpNode*>(expr)){
		HandleInlineNodeOccur(uNode->operand, formalParams, returnVar);
		return;
	}
	if(FunctionalNode *fNode = dynamic_cast<FunctionalNode*>(expr)){
		for(Node *arg : fNode->args)
			HandleInlineNodeOccur(arg, formalParams, returnVar);
		return;
	}
}

void InlineCall::HandleCallNode(Node *&expr){
	if(!expr->isExpression())
		return;
	vector<FuncCallNode*> *container = expr->GetInlineFunction();
	if(!container)
		return;
	Block *block = blocks.top();
	for(int i = 0; i < container->size(); i++){
		FuncCallNode *callNode = container->at(i);
		if(callNode->isRecursiveFuncCallNode())
			continue;
		VarSymbol *var = new VarSymbol("inline_temp" + to_string(inlineCounter), callNode->getType());
		if(!callNode->isVoidFuncCallNode())
			blocks.top()->table->add_symbol(var);
		
		Block *inlineBlock = new Block(*callNode->symbol->body);
		block->body.insert(block->body.begin() + insertIndex + inlineCounter++, dynamic_cast<Node*>(inlineBlock));
		SymTable *formalParams = callNode->symbol->params;
		formalParams->CreateCallParams(/*callNode->args*/);
		HandleBlock(inlineBlock, formalParams, var, inlineBlock->endLabel);
		/*for(int i = 0; i < inlineBlock->body.size(); i++)
			HandleInlineNodeOccur(inlineBlock->body[i], formalParams, var);*/
		if(callNode == expr){
			expr->needDeleteng = true;
			formalParams->InitCallParams(callNode->args, inlineBlock);
		}
		else {
			formalParams->InitCallParams(callNode->args, inlineBlock);
			expr->ReplaceCallNode(callNode, new IdentifierNode(var));
		}
	}
}

void InlineCall::ExecInline(Block *block){
	blocks.push(block);
	inlineCounter = 0;
	const int size = block->size();
	for(int i = 0; i < size; i++){
		Node *expr = block->body[i + inlineCounter];
		if(expr->isExpression()){
			HandleCallNode(expr);
			continue;
		}
		if(ReturnStatement *ret = dynamic_cast<ReturnStatement*>(expr)){
			if(ret->value)
				HandleCallNode(ret->value);
			continue;
		}
		if(Block *containedBlock = dynamic_cast<Block*>(expr)){
			int i = inlineCounter;
			ExecInline(containedBlock);
			inlineCounter = i;
			continue;
		}
		if(CycleStatement *loop = dynamic_cast<CycleStatement*>(expr)){
			int i = inlineCounter;
			ExecInline(dynamic_cast<Block*>(loop->body));
			inlineCounter = i;
			continue;
		}
		if(IfStatement *_if = dynamic_cast<IfStatement*>(expr)){
			int i = inlineCounter;
			ExecInline(dynamic_cast<Block*>(_if->if_branch));
			if(_if->else_branch)
				ExecInline(dynamic_cast<Block*>(_if->else_branch));
			inlineCounter = i;
			continue;
		}
		insertIndex++;
	}
	for(int i = 0; i < block->body.size(); i++){
		if(block->body[i]->needDeleteng){
			delete block->body[i];
			block->body[i] = new EmptyNode();
		}
	}
	blocks.pop();
}