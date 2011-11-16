#ifndef LEAF
#define LEAF
#include "node.h"
#include "../scope.h"
class Leaf_node : public Node
{
	public:
	data_type dt;
	Leaf_node(){}
	
	data_type get_type()
	{
		return dt;
	}
	virtual void set_type(data_type dtt)
	{
		dt = dtt;
	}
	
};

class Var : public Leaf_node
{
	string id;
	t_entry* entry;
	public:
	Var(string idd)
	{
		id = idd;
	}
	t_entry* get_entry()
	{
		return entry;
	}
	void set_entry(t_entry* t)
	{
		entry = t;
		t->node = this;
	}
	int get_line()
	{
		return entry->line;
	}
	string get_id()
	{
		return id;
	}
	void evaluate()
	{
		if(entry->params != NULL) //Just to make sure :-)
			throw "Cannot call a variable as a function!";
	}
	void check_scope(Scope_stack* scope)
	{
		if(scope->search(entry->ID) != entry)
		{
			cout << "Error: Variable " << id
				<< " undeclared in this scope." << endl;
		}
	}
};
class Array : public Leaf_node
{
	unsigned size;
	string id;
	t_entry* entry;
	public:
	Array(string idd, unsigned sizee)
	{
		id = idd; size = sizee;
	}
	void check_scope(Scope_stack* scope)
	{
		if(scope->search(entry->ID) != entry)
		{
			cout << "Array " << id
				<< " undeclared in this scope." << endl;
		}
	}
	int get_line()
	{
		return entry->line;
	}
	string get_id()
	{
		return id;
	}
	void set_entry(t_entry* t)
	{
		entry = t;
		t->node = this;
	}
	void set_type(data_type dtt)
	{
		switch(dtt){
			case IVAL:
				dt = IARRAY;
			case CVAL:
				dt = CARRAY;
			case LVAL:
				dt = LARRAY;
			default:
				cout << "Warning: Unknow type for array!" << endl;
		}
	}
	void set_indexer(Node* n)
	{
		data_type t = entry->type;
		if(t!=CARRAY && t!= LARRAY && t!=IARRAY)
			throw "Unable to convert variable to array.";
		n->evaluate(); //not sure why doing this.
		//TODO
	}
	void set_indexer(int i)
	{
		data_type t = entry->type;
		if(t!=CARRAY && t!= LARRAY && t!=IARRAY)
			throw "Unable to convert variable to array.";
		//TODO 
	}

};

class Type : public Leaf_node
{
	public:
	Type(){}
	Type(data_type t)
	{
		set_type(t);
	}
};

class Const_num : public Leaf_node
{
	int num;
	public:
	Const_num(int n)
	{
		num = n;
	}
	data_type get_type()
	{
		return IVAL; // I'm a int, lol
	}
};
class Parameter : public Var
{ // Parameters are always in the scope of a given function
	// No check_scope is needed here (I guess).
	public:
	Parameter(Type* t, string id) : Var(id)
	{
		set_type(t->get_type());
	}
};
class Array_parameter : public Array
{
	public:
	Array_parameter(Type* t, string id, int sizee) : Array(id, sizee)
	{
		set_type(t->get_type());
	}
};
#endif
