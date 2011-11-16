#ifndef SCOPE_CPP
#define SCOPE_CPP

#include "../include/scope.h"
#include "../include/symbol_table.h"
#include "../include/parse_tree/node.h"

void Scope_level::push(t_entry* entry)
{
	elements.push_back(entry);
}
	
void Scope_level::print()
{
	for(int i=0; i<elements.size(); ++i)
		cout << elements[i]->ID << " ";
}
t_entry* Scope_level::search(string id)
{
	unsigned size = elements.size();
	for(int i=0; i<size; ++i)
		if(elements[i]->ID == id)
			return elements[i];
	return NULL;
}
t_entry* Scope_level::search(t_entry* entry)
{
	unsigned size = elements.size();
	for(int i=0; i<size; ++i)
		if(elements[i] == entry)
			return elements[i];
	return NULL;
}
t_entry* Scope_level::get_last()
{ // Return use this
	return elements.back();
}
/*===========Scope_stack======================*/

/*============ New Level =====================*/
void Scope_stack::push(Scope_level* l)
{
	level.push_back(l);
}
void Scope_stack::push_new_level(Scope_level* l)
{
	push(l);
}
void Scope_stack::push_new_level(t_entry* t)
{
	Scope_level* l = new Scope_level();
	l->push(t);
	push(l);
}
void Scope_stack::push_new_level(vector<t_entry*>* vec)
{
	int size = vec->size();
	Scope_level* l = new Scope_level();
	for(int i=0; i<size; ++i)
		l->push(vec->at(i));
	push(l);
}
/*=========== Same Level =====================*/
void Scope_stack::push_same_level(t_entry* t)
{
	level.back()->push(t);
}
void Scope_stack::push_same_level(vector<t_entry*>* vec)
{
	int size = vec->size();
	Scope_level* l = level.back();
	for(int i=0; i<size; ++i)
		l->push(vec->at(i));

	delete vec; // should check this again... FIXME
}
/*============== Others =====================*/
Scope_level* Scope_stack::pop() // Always pop an entire level
{
	Scope_level* r = level.back();
#ifdef SHOW_SCOPE
	print();
#endif
	level.pop_back();
	return r;
}
t_entry* Scope_stack::search(string id)
{
	vector<Scope_level*>::iterator it;
	vector<Scope_level*>::iterator begin;
	it = level.end() - 1;
	begin = level.begin(); //Global scope
	t_entry* ret;
	for(; it != begin; --it)
	{
		ret = (*it)->search(id);
		if(ret)
			return ret;
	}
	return (*begin)->search(id); // Null or not
}
t_entry* Scope_stack::search(t_entry* entry)
{
	vector<Scope_level*>::iterator it;
	vector<Scope_level*>::iterator begin;
	it = level.end() - 1;
	begin = level.begin(); //Global scope
	t_entry* ret;
	for(; it != begin; --it)
	{
		ret = (*it)->search(entry);
		if(ret)
			return ret;
	}
	return (*begin)->search(entry); // Null or not
}
t_entry* Scope_stack::search_current_function()
{
	// The current function should be the last element
	// inserted in the GLOBAL level.
	

	t_entry* ret = level.front()->get_last();
	if(ret->params) // Make sure it is a function
		return ret;

	cout << "Error! Return statement couldn't find it's function"
		<< " Scope_stack::search_current_function()" << endl;
	return NULL;
}
void Scope_stack::print()
{
	unsigned size = level.size();
	for(int i=0; i<size; ++i)
	{
		cout << "Level " << i << ":" << endl;
		level[i]->print();
		cout << endl;
	}
	cout << "-----------------------" << endl;
}

#endif
