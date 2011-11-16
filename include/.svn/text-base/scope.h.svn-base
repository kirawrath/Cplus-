#ifndef SCOPE_H
#define SCOPE_H
#include "symbol_table.h"
#include "parse_tree/node.h"

class Scope_level
{
	vector<t_entry*> elements;
	public:
	void push(t_entry* entry);
	t_entry* search(string id);
	t_entry* search(t_entry* entry);
	t_entry* get_last();
	void print();
};

/*===========Scope_stack======================*/

class Scope_stack
{
	vector<Scope_level*> level;
	public:
	void push(Scope_level* l);

	void push_new_level(Scope_level* l);
	void push_new_level(t_entry* t);
	void push_new_level(vector<t_entry*>*);

	void push_same_level(t_entry* t);
	void push_same_level(vector<t_entry*>*);

	Scope_level* pop();
	t_entry* search(string id);
	t_entry* search(t_entry* entry);
	t_entry* search_current_function(); // to Return;
	void print(); //Debug purposes
};

#endif
