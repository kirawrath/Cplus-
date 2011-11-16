#ifndef COMMAND
#define COMMAND
#include "leaf.h"
#include "declarations.h"
extern int yylineno;
class Command : Node
{
	public:
	Command(){}
};
class Attribution : public Node
{
	public:
	Attribution(Node* n0, op_type op_t) : Node(n0)// Unary
	{
	}
	Attribution(Node* n0, Node* n1) : Node(n0,n1) //Binary
	{
	}
	void evaluate()
	{
		child[0]->evaluate();
		if(child.size()>1)
		{
			child[1]->evaluate();
			if(child[0]->get_type() != child[1]->get_type())
				puts("Incompatible types in assignment");
#ifdef DEBUG
			cout << child[0]->get_type() << endl;
			cout << child[1]->get_type() << endl;
#endif
		}
	}
};
class Lvalue_list : public Node
{
	data_type expected_type;
	public:
	Lvalue_list(Node* n) : Node(n)
	{
		expected_type = NOTYPE;
	}

	void evaluate()
	{
		//There is at least one node
		if(!child[0])
			return;

		expected_type = child[0]->get_type(); 

		unsigned size = child.size();
		for(int i=1; i<size; ++i)
		{
			try{
				if(child[i]->get_type() != expected_type)
				{
/*				std::stringstream ss;
				ss << "Incompatible type in assignment in line ";
				ss << child[i]->get_line();
				ss << " with " << child[i]->get_id() << ".";

				throw ss.str();*/
					throw "Incompatible type in assignment";
				}
			}
			catch(const char* problem)
			{
				cout << problem << endl;
			}
		}

	}
	data_type get_type()
	{
#ifdef DEBUG
		cout << "lvalue_list type: "<< expected_type << endl;
#endif
		return expected_type;
	}

};
class Selection : public Node
{
	public:
	Selection(Expression* n0, Node* n1) : Node(n0,n1){}
	Selection(Expression* n0, Node* n1, Node* n2) : Node(n0,n1,n2){}
};
class Iteration : public Node
{
	public:
	Iteration(Node* n0) : Node(n0){//forever
		add_implicit_i();}
	Iteration(Node* n0, Node* n1) : Node(n0,n1){
		add_implicit_i();}
	Iteration(Node* n0, Node* n1, Node* n2, Node* n3) : Node(n0,n1,n2,n3){
		add_implicit_i();}
	private:
	void add_implicit_i()
	{
		
	}
};
class Return : public Node
{
	data_type dt;

	public:
	data_type get_type(){return dt;}
	void set_type(data_type dtt)
	{
		dt = dtt;
	}
	Return() : Node(){}
	Return(Node* n) : Node(n){
		dt = NOTYPE;
	}

	void evaluate()
	{
		if(child.size()>0)
		{
			if(child[0]->get_type() != dt)
				cout << "Warning: Return statement in line "
					 << yylineno <<" should "
					 << "return " << type_str(child[0]->get_type())
					 << " but it is returning "
					 << type_str(dt) << " instead." << endl;
		}
	}
	void check_scope(Scope_stack* scope)
	{
		t_entry* t = scope->search_current_function();
		if(t)
			set_type(t->type);
	}
};

class Arguments : public Node
{
	public:
	Arguments(Expression* n) : Node(n)
	{}
	Arguments() : Node()
	{}
	unsigned size()
	{
		return child.size();
	}
	data_type type(int i)
	{
		return child[i]->get_type();
	}
};

class Function_call : public Node
{
	t_entry* func;
	Arguments* args;
	public:
	Function_call(Arguments* arg, t_entry* function)
	{
		func = function;
		args = arg;
	}
	void Evaluate()
	{
		Parameter_list* params = (Parameter_list*)func->node;

		unsigned size = args->size();
		if(size != params->size())
			throw "Incompatible number of parameters";

		for(int i=0; i<size; ++i)
		{
			if(params->type(i) != args->type(i))
				throw "Incompatible types given as arguments to function";
		}
	}
	string get_id()
	{
		return func->ID;
	}

};
#endif
