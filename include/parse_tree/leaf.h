#ifndef LEAF
#define LEAF
#include "node.h"
#include "../scope.h"
extern int yylineno;
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
	protected:
	string id;
	t_entry* entry;
	public:
	Var(string idd)
	{
		id = idd;
		entry = NULL;
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
		if(!entry)
			cout << "Variable "<< id << " hasn't an entry!" << endl;
		if(scope->search(entry) != entry)
		{
			cout << "Error: Variable " << id
				<< " undeclared in this scope." << endl;
		}
	}
};
class Array : public Var
{
	unsigned size;
	int indexer;
	Node* n_indexer;
	public:
	Array(string idd, unsigned sizee) : Var(idd)
	{
		size = sizee;
		n_indexer = NULL;
		indexer = 0;
	}
	void check_scope(Scope_stack* scope)
	{
		if(scope->search(entry->ID) != entry)
		{
			cout << "Array " << id
				<< " undeclared in this scope." << endl;
		}
	}
	void set_type(data_type dtt)
	{
		switch(dtt){
			case IVAL:
				dt = IARRAY;
				entry->type = IARRAY;
				break;
			case CVAL:
				dt = CARRAY;
				entry->type = CARRAY;
				break;
			case LVAL:
				dt = LARRAY;
				entry->type = LARRAY;
				break;
			default:
				cout << "Warning: Unknow type for array! " << dtt <<endl;
		}
	}
	data_type get_element_type()
	{
		switch(dt){
			case IARRAY:
				return IVAL;
			case CARRAY:
				return CVAL;
			case LARRAY:
				return LVAL;
		}
		cout << "No element type!"
			 << " My type is " << entry->type;
			 //<< " My id is " << entry->ID << endl;
	}
	void set_indexer(Node* n)
	{
	/*	data_type t = entry->type;
		if(t!=CARRAY && t!= LARRAY && t!=IARRAY)
			throw "Unable to convert variable to array.";*/
		n->evaluate(); //not sure why doing this.
		n_indexer = n;
	}
	
	void set_indexer(int i)
	{
		/*data_type t = entry->type;
		if(t!=CARRAY && t!= LARRAY && t!=IARRAY)
			throw "Unable to convert variable to array.";*/
		if( i >= size )
			cout << "Warning on line "<<yylineno<<": indexer for "<<entry->ID
				<< " is higher than it's size.";
		indexer = i;
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
class Const_char : public Leaf_node
{
	char ch;
	public:
	Const_char(char charlie)
	{
		ch = charlie;
	}
	data_type get_type()
	{
		return CVAL;
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
