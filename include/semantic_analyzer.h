#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "parse_tree/node.h"
#include "symbol_table.h"
class Semantic_analyzer
{
	Node* root;
	Symbol_table* st;
	public:
	Semantic_analyzer(Node* tree_root, Symbol_table* t);
	void analyze();
};
#endif
