#ifndef DECLARATIONS
#define DECLARATIONS
#include "node.h"
#include "vector"
#include "../scope.h" 

class Declarations : public Node
{
	public:
	Declarations(Node* n) : Node(n)
	{}
};
class Id_list : public Node
{
	public:
	Id_list(Var* n): Node(n) {}

	vector<t_entry*>* entries()
	{
		unsigned size = child.size();
		vector<t_entry*>* entries = new vector<t_entry*>;

		for(int i=0; i<size; ++i)
			entries->push_back( child[i]->get_entry() );

		return entries;
	}
	void check_scope(Scope_stack* scope)
	{
		unsigned size = child.size();
		for(int i=0; i<size; ++i)
			((Var*)child[i])->build_entry();
	}
};
class Var_declaration : public Node
{
	vector<t_entry*>* entries;
	data_type dt;
	public:
	Var_declaration(Type* t, Id_list* il) : Node(t, il)
	{
		dt = child[0]->get_type();
	}
	data_type get_type()
	{
		return dt;
	}
	void evaluate()
	{
		Id_list* ids = (Id_list*) child[1];
		unsigned size = ids->child.size();

		for(int i=0; i<size; ++i)
		{
			try{
				ids->child[i]->set_type(dt);
			}catch(const char* problem){
				cout << problem << endl;
				cout << child[i]->get_id() << endl;
				cout << "Var_declaration::evaluate()" << endl;
			}
		}
	}
	void check_scope(Scope_stack* scope)
	{
		child[1]->check_scope(scope);
	}
	vector<t_entry*>* get_entries()
	{
		Id_list* idl = (Id_list*)child[1];
		return idl->entries();
	}
	void gen_code(Code_gen* gen)
	{
		Id_list* ids = (Id_list*) child[1];
		unsigned size = ids->child.size();
		switch(dt)
		{
			case IVAL:
				for(int i=0; i<size; ++i)
				{	
					gen->write("\tldc 0\n");
					int n = gen->get_counter();
					gen->write("\tistore ");
					gen->write(n);
					gen->write(" ; initializing with zero\n");
					((Var*)ids->child[i])->set_register(n);
					gen->inc_counter();
				}
				break;
		}
	}
};

class Vars_declarations : public Node
{//var_decls
	public:
	Vars_declarations() : Node()
	{}
	unsigned child_size()
	{
		return child.size();
	}
	void check_scope(Scope_stack* scope)
	{
#ifdef DEBUG
cout << "Vars_declarations::check_scope()1" << endl;
#endif
		unsigned size = child.size();
		for(int i=0; i<size; ++i) // To create the entries.
			child[i]->check_scope(scope);

		// Pushing the entries to the current scope level.
		for(int i=0; i<size; ++i)
		{
			entry_vec vec =
				((Var_declaration*)child[i])->get_entries();
			scope->push_same_level(vec);
		}
		entry_vec repeated = scope->
			search_multiple_entries();
		if(repeated)
			for(unsigned i=0; i<repeated->size(); ++i)
				cout << "Error: Multiple declarations for "
					 << repeated->at(i)->ID << "." << endl;

#ifdef DEBUG
cout << "Vars_declarations::check_scope()2" << endl;
#endif
	}
};
class Parameter_list : public Node
{
	public:
	Parameter_list() : Node()
	{}
	Parameter_list(Node* n) : Node(n)
	{}
	unsigned size()
	{
		return child.size();
	}
	data_type type(int i)
	{
		return child[i]->get_type();
	}
	vector<t_entry*>* get_entries()
	{
		unsigned size = child.size();
		vector<t_entry*>* vec = new vector<t_entry*>;
		for(int i=0; i<size; ++i)
			vec->push_back(child[i]->get_entry());
		return vec; // May have size 0
	}
	void check_scope(Scope_stack* dummy)
	{
		unsigned size = child.size();
		for(int i=0; i<size; ++i)
			((Var*)child[i])->build_entry();
	}
};

class Block : public Node
{
	protected:
	entry_vec func_params;
	public:
	Block(Node* n0, Node* n1) : Node(n0, n1)
	{
		func_params = NULL;
	}
	void set_func_params(entry_vec p) // Used by Function and Iteration classes
	{
		func_params = p;
	}
	void check_scope(Scope_stack* scope)
	{
#ifdef DEBUG
		cout << "Block::check_scope()1" << endl;
#endif
		Scope_level* level = new Scope_level();
		scope->push_new_level(level);
		
		if(func_params)
			scope->push_same_level(func_params);

		unsigned size = child.size(); //2
		for(int i=0; i<size; ++i)
			child[i]->check_scope(scope);

		scope->pop();
#ifdef DEBUG
		cout << "Block::check_scope()2" << endl;
#endif
	}
};
class Cmd_list : public Block // A one command block (used by FOR and IF)
{
	public:
	Cmd_list(Node* n0) : Block(new Null_node(), n0)
	{
		func_params = NULL;
	}
};
class Function : public Node
{
	t_entry* entry;
	string id;
	int line;
	
	public:
	Function(Node* n0, Parameter_list* n1, Node* n2) : Node(n0, n1, n2)
	{
		entry = NULL;
	}
	void set_id(string idd)
	{
		id = idd;
	}
	void set_line(int l)
	{
		line = l;
	}
	int get_line()
	{
		return line;
	}
	void build_entry()
	{
		assert(!entry);
		
		entry = new t_entry;
		entry->ID = id;
		entry->line = line;
		entry->node = this;
		entry->params = (Parameter_list*)child[1];
		entry->type = child[0]->get_type();
	}
	void evaluate()
	{
		try{
			if(child.size()<3)
				throw get_id();
			child[1]->evaluate();
			child[2]->evaluate();
		}
		catch(string s){
			cout<<"Error while parsing function "<< s <<endl;
		}
	}
	void check_scope(Scope_stack* scope)
	{
		build_entry();
		// push itself into scope:
		scope->push_same_level(get_entry());
		child[1]->check_scope(scope); // To build the parameters entries.
		// push param list to scope
		entry_vec p = ((Parameter_list*)child[1])->get_entries();
		((Block*)child[2])->set_func_params(p);
		// push block vars to scope
		// push if and for blocks to scope
		child[2]->check_scope(scope); // Block
	}
	string get_id()
	{
		return id;
	}
	data_type get_type()
	{
		return entry->type;
	}
	void set_type(data_type dtt)
	{
		entry->type = dtt;
	}
	void set_entry(t_entry* t, Parameter_list* p )
	{
		entry = t;
		t->node=this;
		if(p)
			t->params = p;
		else
			cout << "WHAT HELL ARE YOU DOING?! (Function)"<<endl;
	}
	void set_entry(t_entry* t)
	{
		t->node=this;
		entry = t;
		t->params = (Parameter_list*)child[1];
		set_type(child[0]->get_type());
	}
	t_entry* get_entry()
	{
		return entry;
	}
};
#endif
