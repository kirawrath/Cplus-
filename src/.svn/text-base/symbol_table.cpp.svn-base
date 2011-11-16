#ifndef SYMBOL_TABLE_CPP
#define SYMBOL_TABLE_CPP
#include "../include/symbol_table.h"
#include "../include/parse_tree/node.h"
#include "../include/parse_tree/null_node.h"

using namespace std;
Symbol_table::Symbol_table(){}

t_entry* Symbol_table::insert(t_entry* t)
{
	unsigned n = entries.size();
	for(int i=0; i<n; ++i)
		if(entries[i]->ID == t->ID)
		{
			delete t;
			return entries[i];
		}
	entries.push_back(t);
	return t;
}
t_entry* Symbol_table::get_entry(string id)
{
	unsigned n = entries.size();
	for(int i=0; i<n; ++i)
		if(entries[i]->ID == id)
			return entries[i];
	return NULL;		
}
Node* Symbol_table::get_node(string id)
{
	unsigned n = entries.size();
	for(int i=0; i<n; ++i)
		if(entries[i]->ID == id)
			return entries[i]->node;
	cout << "Variable "<<id<<" undeclared." << endl;

	return new Null_node();
}
string type_str(data_type dt)
{
	switch(dt)
	{
	case LVAL:
		return "extremeint";
	case CVAL:
		return "charlie";
	case IVAL:
		return "int";
	case IARRAY:
		return "int[]";
	case LARRAY:
		return "extremeint[]";
	case CARRAY:
		return "charlie[]";
	case NOTYPE:
		return "No Type";
	default:
		return "\"Could not find the given type\"";
	}
}
#endif
