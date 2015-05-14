#include "h_otimizer.h"

SubExpressionTap::SubExpressionTap() : temp_counter(0) {}

void SubExpressionBlock::Add(Values value, Node *node){
	auto opList = expressions[value];
	if(!opList)
		opList = expressions[value] = new vector<Node*>;
	
	for(Node *n : *opList)
		if(*n == node){
			node->commonNode = n;
			break;
		}
	opList->push_back(node);
}

void SubExpressionBlock::Add(Node *node){
	auto currentOp = expressions[node->value()];
	if(!currentOp)
		currentOp = expressions[node->value()] = new vector<Node*>;

	for(Node *n : *currentOp)
		if(*node == n){
			node->commonNode = n;
			break;
		}
	currentOp->push_back(node);
}

bool SubExpressionTap::IsChangedValue(Values v) const {
	return v == incr || v == decr	 ||
		v == assign  || v == pl_eql  ||
		v == mn_eql	 || v == fct_eql ||
		v == div_eql || v == per_eql ||
		v == and_eql || v == or_eql	 ||
		v == xor_eql || v == not_b_eql ||
		v == sl_eql  || v == sr_eql;
}

bool SubExpressionTap::IsModified(Node *node) const {
	Values value = node->value();
	return !dynamic_cast<FunctionalNode*>(node) &&
		!IsChangedValue(value) && value != quest;
}

bool SubExpressionTap::isTepmInit(Node *node) const {
	if(BinaryOpNode *bNode = dynamic_cast<BinaryOpNode*>(node))
			if(IdentifierNode *iNode = dynamic_cast<IdentifierNode*>(bNode->left))
				if(iNode->assignCounter == -1)
					return true;
	return false;
}

bool SubExpressionTap::GetArgs(SubExpressionBlock &expBlock, Node *node) {
	if(!node)
		return false;
	Values value = node->value();
	if(BinaryOpNode *bNode = dynamic_cast<BinaryOpNode*>(node)){
		if(IdentifierNode *l_id = dynamic_cast<IdentifierNode*>(bNode->left))
				l_id->assignCounter = IsChangedValue(value) ? ++l_id->var->assignCounter : l_id->var->assignCounter;			
		if(IdentifierNode *r_id = dynamic_cast<IdentifierNode*>(bNode->right)){
			r_id->assignCounter = r_id->var->assignCounter;
		}

		if(!((bNode->left->isIdent() || bNode->left->isSimple()) &&	(bNode->right->isIdent() || bNode->right->isSimple()))){
			if(!IsModified(bNode->left) || !IsModified(bNode->right))
				return false;
				
			bool ok1 = GetArgs(expBlock, bNode->left),
				ok2 = GetArgs(expBlock, bNode->right);
			if(!ok1 || !ok2 || !IsModified(node))
				return false;
			expBlock.Add(node);
		} else {
			expBlock.Add(node);
		if(!IsModified(node))
			return false;
		}
	} else
		if(UnaryOpNode *uNode = dynamic_cast<UnaryOpNode*>(node)){
			if(IdentifierNode *id = dynamic_cast<IdentifierNode*>(uNode->operand)){
				if(IsChangedValue(value))
					id->assignCounter = ++id->var->assignCounter;
				else 
					id->assignCounter = id->var->assignCounter;
			}
			if(!(uNode->operand->isSimple() || uNode->operand->isIdent())){
				if(!GetArgs(expBlock, uNode->operand) || !IsModified(node))
					return false;
				expBlock.Add(node);
			}
			else
				expBlock.Add(uNode->value(), uNode->operand);
		}
	return true;
}

void SubExpressionTap::FindCommonExpr(Block *block) {
	SubExpressionBlock *exprList = new SubExpressionBlock;
	blocks.push(block);
	block->initSubExprBlock(exprList);
	
	for(int i = 0; i < block->body.size(); i++){
		Node *expr = block->body[i];
		expr->SetIndex(i);
		if(FuncCallNode *cNode = dynamic_cast<FuncCallNode*>(expr)){
			for(Node *arg : cNode->args)
				GetArgs(*exprList, arg);
			continue;			
		}
		if(expr->isExpression()){
			GetArgs(*exprList, expr);
			continue;
		}
		if(expr->isCycle())
			continue;
		if(Block* b = dynamic_cast<Block*>(expr)){
			FindCommonExpr(b);
			continue;
		}
		if(IfStatement *$if = dynamic_cast<IfStatement*>(expr)){
			FindCommonExpr(dynamic_cast<Block*>($if->if_branch));
			if($if->else_branch)
				FindCommonExpr(dynamic_cast<Block*>($if->else_branch));
			continue;
		}
	
		if(ReturnStatement *retNode = dynamic_cast<ReturnStatement*>(expr)){
			GetArgs(*exprList, retNode->value);
			continue;
		}
	}
	blocks.pop();
}

void SubExpressionTap::CreateNewIdent(Node *node){
	if(node->opt)
		return;
	node->opt = true;
	VarSymbol *var = new VarSymbol(string("temp" + to_string(temp_counter)), node->getType());
	blocks.top()->table->add_symbol(var);
	Token *assignOp = new Operation(assign);
	Node *id_token = new IdentifierNode("temp" + to_string(temp_counter), var);
	Node *t = new BinaryOpNode(assignOp, id_token, node);
	dynamic_cast<IdentifierNode*>(dynamic_cast<BinaryOpNode*>(t)->left)->assignCounter = -1;

	blocks.top()->body.insert(blocks.top()->body.begin() + node->index + temp_counter, t);
	temp_counter++;
	node->replaced = id_token;
}

Node* SubExpressionTap::OptimizeExpression(Node *node){
	Block *block = blocks.top();
	SubExpressionBlock *expBlock = block->exprBlock;
	vector<Node*> *opList = expBlock->expressions[node->value()];
	if(opList)
		for(Node *expr : *opList){
			if(expr == node){
				if(expr->commonNode){
					if(*expr->commonNode == expr){
						CreateNewIdent(expr->commonNode);
						return expr->commonNode->replaced;
					}
					return expr;
				}
			}
		}
	if(BinaryOpNode *bNode = dynamic_cast<BinaryOpNode*>(node)){
		Node *l_opt = OptimizeExpression(bNode->left),
			 *r_opt = OptimizeExpression(bNode->right);
		if(bNode->left != l_opt)
			bNode->left = l_opt;
		
		if(bNode->right != r_opt)
			bNode->right = r_opt;
	} else
		if(UnaryOpNode *uNode = dynamic_cast<UnaryOpNode*>(node)){
			bool opt = false;
			for(Node *expr : *opList){
				if(uNode->operand == expr){
					opt = true;
					if(uNode->operand->commonNode)
						CreateNewIdent(uNode->operand->commonNode);
					Node *optNode = OptimizeExpression(uNode->operand);
					if(optNode != uNode->operand){
						delete uNode->operand;
						uNode->operand = optNode;
					}
					break;
				}
			}
			if(!opt)
				OptimizeExpression(uNode->operand);
		}
	return node;
}

void SubExpressionTap::ReplaceFirstOccur(){
	Block *block = blocks.top();
	for(int i = 0; i < block->body.size(); i++){
		if(!isTepmInit(block->body[i]))
			block->body[i] = ReplaceFirstOccurInNode(block->body[i]);
	}
}

Node* SubExpressionTap::ReplaceFirstOccurInNode(Node *node){
	if(node->replaced)
		return node->replaced;
	if(BinaryOpNode *bNode = dynamic_cast<BinaryOpNode*>(node)){
		if(bNode->left->replaced)
			bNode->left = bNode->left->replaced;
		else
			bNode->left = ReplaceFirstOccurInNode(bNode->left);
		if(bNode->right->replaced)
			bNode->right = bNode->right->replaced;
		else
			bNode->right = ReplaceFirstOccurInNode(bNode->right);
	}
	if(UnaryOpNode *uNode = dynamic_cast<UnaryOpNode*>(node)){
		if(uNode->operand->replaced)
			uNode->operand = uNode->operand->replaced;
		else
			uNode->operand = ReplaceFirstOccurInNode(uNode->operand);
	}

	return node;
}

void SubExpressionTap::Optimize(Block *block){
	blocks.push(block);
	SubExpressionBlock *exprList = block->GetExprBlock();
	const int size = block->body.size();
	for(int i = 0; i < size; i++){
		Node *expr = block->body[i + temp_counter];
		if(expr->isCycle())
			continue;
		if(Block* b = dynamic_cast<Block*>(expr)){
			Optimize(b);
			continue;
		}
		if(FuncCallNode *cNode = dynamic_cast<FuncCallNode*>(expr)){
			for(Node *arg : cNode->args)
				GetArgs(*exprList, arg);
			continue;			
		}
		if(IfStatement *$if = dynamic_cast<IfStatement*>(expr)){
			Optimize(dynamic_cast<Block*>($if->if_branch));
			if($if->else_branch)
				Optimize(dynamic_cast<Block*>($if->else_branch));
			continue;
		}
		if(ReturnStatement *retNode = dynamic_cast<ReturnStatement*>(expr))
			expr = retNode->value;
		if(!expr->isExpression())
			continue;
		Node *t = OptimizeExpression(expr);
		if(t != expr){
			block->body[i + temp_counter] = t;		
		}
	}
	ReplaceFirstOccur();
	blocks.pop();
}
