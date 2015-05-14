#include "h_otimizer.h"

HOptimizer::HOptimizer(Node *_tree) : tree(_tree), optimized(false), current(_tree), argIndex(0) {}

void HOptimizer::replaceNode(Node *_new, Node *_replaced, Place p){
	optimized = true;
	delete _replaced;
	switch(p){
	case Root:
		tree = _new;
		 break;
	case BinLeft:
		dynamic_cast<BinaryOpNode*>(current)->left = _new;
		break;
	case BinRight:
		dynamic_cast<BinaryOpNode*>(current)->right = _new;
		break;

	case UnOperand:
		dynamic_cast<UnaryOpNode*>(current)->operand = _new;
		break;

	case FuncArg:
		dynamic_cast<FunctionalNode*>(current)->args[argIndex] = _new;
		break;
	case BaseofTern:
		dynamic_cast<TernaryOpNode*>(current)->condition = _new;
		break;
	case Ident:
		dynamic_cast<IdentifierNode*>(current)->var->init = _new;
		break;
	}
}

bool HOptimizer::OptimizationComplete(){
	bool ret = !optimized;
	optimized = false;
	return ret;
}

Node* HOptimizer::result(){
	return tree;
}

void HOptimizer::constFolding(Node *node, Place p){
	if(node->isSimple())
		return;
	if(BinaryOpNode *bNode = dynamic_cast<BinaryOpNode*>(node)){
		if(TernaryOpNode *tNode = dynamic_cast<TernaryOpNode*>(node)){
			 if(tNode->condition->isSimple())
				 replaceNode(tNode->calculate(), node, p);
			 else
			 {
				 Node *c = current;
				 current = tNode;
				 constFolding(tNode->condition, BaseofTern);
				 current = c;
				 if(tNode->condition->isSimple())
					 replaceNode(tNode->calculate(), node, p);
			 }
			 return;		
		}
		
		if(bNode->left->isSimple() && bNode->right->isSimple()){
			replaceNode(bNode->BinaryOpNode::calculate(), node, p);
			return;
		}
		else {
			if(!bNode->left->isSimple()){
				Node *c = current;
				current = bNode;
				constFolding(bNode->left, BinLeft);
				current = c;
			}
			
			if(!bNode->right->isSimple()){
				Node *c = current;
				current = bNode;
				constFolding(bNode->right, BinRight);
				current = c;	
			}
			if(bNode->left->isSimple() && bNode->right->isSimple())
				replaceNode(bNode->BinaryOpNode::calculate(), node, p);
		}
		return;
	} else

	if(UnaryOpNode *uNode = dynamic_cast<UnaryOpNode*>(node)){
		if(uNode->operand->isSimple())
			replaceNode(uNode->calculate(), node, p);
		else {
			Node *c = current;
			current = uNode;
			constFolding(uNode->operand, UnOperand);
			current = c;
			if(uNode->operand->isSimple())
				replaceNode(uNode->calculate(), node, p);
		}
		return;
	} else

	if(FunctionalNode *fNode = dynamic_cast<FunctionalNode*>(node)){
		Node *c = current;
		current = fNode;
		int base_index = argIndex;
		for(int i = 0; i < fNode->args.size(); i++){
			argIndex = i;
			constFolding(fNode->args[i], FuncArg);
		}
		argIndex = base_index;
		current = c;
		return;
	} else

	if(IdentifierNode *iNode = dynamic_cast<IdentifierNode*>(node)){
		if(iNode->var->type->isConstSymbol()){
			if(iNode->var->init->isSimple())
				replaceNode(iNode->var->init, node, p);
			else {
				Node *c = current;
				current = iNode;
				constFolding(iNode->var->init, Ident);
				current = c;
				if(iNode->var->init->isSimple())
					replaceNode(iNode->var->init->calculate(), node, p);
			}
		}
	}
}
