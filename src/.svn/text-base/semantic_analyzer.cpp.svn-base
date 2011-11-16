#ifndef SEMANTIC_CPP
#define SEMANTIC_CPP
#include "../include/semantic_analyzer.h"
#include "../include/parse_tree/node.h"
#include "../include/symbol_table.h"

Semantic_analyzer::Semantic_analyzer(Node* tree_root, Symbol_table* t)
{
	root = tree_root;
	st = t;
}
void Semantic_analyzer::analyze()
{
	try
	{
		root->evaluate();
	}
	catch(string problem)
	{
		cout << problem << endl;
	}
	catch(const char* problem)
	{
		cout << problem << endl;
	}

	cout << endl << "Done! \\o/" << endl;
}

#endif
