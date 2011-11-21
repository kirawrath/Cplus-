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
	int line;
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
	}
	void set_type(data_type dtt)
	{
		entry->type = dtt;
	}
	data_type get_type()
	{
		if(!entry) // This is the case when you are working with an
				   // undeclared variable.
			return NOTYPE; 
		return entry->type;
	}
	void build_entry() // Called ONLY by Id_list
	{
		if(entry)
			cout << "Trying to build an already existent entry!" << endl;
		entry = new t_entry;
		entry->ID = id;
		entry->line = line;
	}
	void set_line(int l){line = l;}
	int get_line()
	{
		return line;
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
		{
			entry = scope->search(id);
			if(!entry)
			{
				cout << "Error: Variable " << id
					 << " on line " << line
				 	 << " undeclared in this scope." << endl;
			}
#ifdef DEBUG
			else
				cout << id << " found! Line "<<line<<" (var::check_scope())"<<endl;
			cout << entry << endl;
#endif
		}
		
	}
};
class Array : public Var
{
	unsigned size;
	int indexer;
	Node* n_indexer;
	public:
	Array(string idd, unsigned sizee=0) : Var(idd)
	{
		size = sizee;
		n_indexer = NULL;
		indexer = 0;
	}
	void set_type(data_type dtt)
	{
		switch(dtt){
			case IVAL:
				entry->type = IARRAY;
				break;
			case CVAL:
				entry->type = CARRAY;
				break;
			case LVAL:
				entry->type = LARRAY;
				break;
			default:
				cout << "Warning: Unknow type for array! " << dtt <<endl;
		}
		//cout << "Type set for " << id << " on line " << line << endl;
		//cout << entry << endl;
	}
	data_type get_element_type()
	{
		switch(get_type()){
			case IARRAY:
				return IVAL;
			case CARRAY:
				return CVAL;
			case LARRAY:
				return LVAL;
		}
		cout << "No element type!"
			 << " My type is " << entry->type;
	}
	void set_indexer(Node* n)
	{
		n->evaluate(); //not sure why doing this.
		n_indexer = n;
	}
	
	void set_indexer(int i)
	{
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
