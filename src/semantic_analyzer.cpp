#ifndef SEMANTIC_CPP
#define SEMANTIC_CPP
#include "../include/semantic_analyzer.h"
#include "../include/parse_tree/node.h"
#include "../include/symbol_table.h"
#include "../include/scope.h"

Semantic_analyzer::Semantic_analyzer(Node* tree_root)
{
	root = tree_root;
}
void Semantic_analyzer::analyze()
{
	try
	{
		Scope_stack* scope = new Scope_stack();
		scope->push_new_level(new Scope_level()); // Pushing the global scope
		
		root->check_scope(scope);
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

	cout << "Semantic Analysis Complete! \\o/" << endl;
}

#endif
