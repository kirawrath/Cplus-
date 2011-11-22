#ifndef COMMAND
#define COMMAND
#include "leaf.h"
#include "declarations.h"
extern int yylineno;
class Command : public Node
{
	public:
	Command(){}
};
class Lvalue_list : public Node
{
	data_type expected_type;
	public:
	Lvalue_list(Node* n) : Node(n)
	{
		expected_type = NOTYPE;
	}
	int get_line()
	{
		return ((Var*)child.back())->get_line();
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
					cout << "Incompatible type in assignment on line ";
					cout << child[i]->get_line();
					cout << ". Got " << type_str(child[i]->get_type())
						 << " but " << type_str(expected_type) << " was expected."
						 << endl;
				}
			}
			catch(const char* problem)
			{
				cout << problem << endl;
			}
		}

	}
	void gen_code(Code_gen* gen)
	{		
		int n = ((Var*)child[0])->get_register();
		gen->write("\tistore ");
	   	gen->write( n );
		gen->write("\n");

		unsigned size = child.size();
		for(int i=1; i<size; ++i)
		{
			gen->write("\tiload ");
		   	gen->write(n);
			gen->write("\n");
			gen->write("\tistore ");
			n = ((Var*)child[i])->get_register();
		   	gen->write( n );
			gen->write("\n");
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

class Attribution : public Node
{
	op_type op;
	bool is_array(data_type dt)
	{
		if(dt == CARRAY || dt == LARRAY || dt == IARRAY)
			return true;
		return false;
	}
	bool compatible(Node* n0, Node* n1)
	{
		data_type d0 = n0->get_type();
		data_type d1 = n1->get_type();
		
		if(d0 == d1)
			return true;

		if(is_array(d0))
			if(((Array*)n0)->get_element_type() == d1)
				return true;

		if(is_array(d1))
			if(((Array*)n1)->get_element_type() == d0)
				return true;

		return false;
	}

	public:
	Attribution(Node* n0, op_type op_t) : Node(n0)// Unary
	{
		op = op_t;
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
			if( ! compatible(child[0], child[1]))
			{
				cout<<"Incompatible types in assignment ("
					<< type_str(child[0]->get_type()) << ", "
					<< type_str(child[1]->get_type()) << ")"
					<<" on line "
					<< ((Lvalue_list*)child[0])->get_line() << endl;
			}
		}
		else // Unary operator like ++ -- +-
		{
			for(unsigned i=0; i<child.size(); ++i)
				if(child[i]->get_type() != IVAL)
					if(child[i]->get_type() != LVAL) // Fuck you branch prediction!
						cout << "Error: Incompatible type for unary operator"
							 << " on line "<< yylineno << "." << endl;
		}
	}
	void gen_code(Code_gen* gen)
	{
		if(child.size() > 1)
		{
			child[1]->gen_code(gen);
			child[0]->gen_code(gen);
		}
		else // Unary
		{
			Var* c = (Var*)child[0];
			int r = c->get_register();
			switch(op)
			{
				case INCL:
				case INCR:
					c->gen_code(gen);
					gen->write("ldc 1\n");
					gen->write("iadd\n");
					gen->write("istore ", r);
					break;
				case DECL:
				case DECR:
					c->gen_code(gen);
					gen->write("ldc 1\n");
					gen->write("isub\n");
					gen->write("istore ", r);
					break;
				case OITOKL:
				case OITOKR:
					string l1 = gen->new_label(),
						   l2 = gen->new_label();
					c->gen_code(gen);
					gen->write("ldc 8000\n");
					gen->write("isub\n");
					gen->write("ifgt " + l1 + "\n");															
					gen->write("ldc 0\n");
					gen->write("goto " + l2 + "\n");
					gen->write(l1 + ":" + "\n");
					gen->write("ldc 1\n");
					gen->write(l2 + ":" + "\n");
			}
		}
	}
};
class Selection : public Node
{
	public:
	Selection(Expression* n0, Node* n1) : Node(n0,n1){}
	Selection(Expression* n0, Node* n1, Node* n2) : Node(n0,n1,n2){}

	void gen_code(Code_gen* gen)
	{
		child[0]->gen_code(gen);
		string l = gen->new_label();

		gen->write("\tifeq " + l + "\n");
		child[1]->gen_code(gen);
		if(child.size()<=2) // if expression
		{
			gen->write("\t" + l + ":" + "\n");
		}
		else //if expression else expression
		{
			string end = gen->new_label();
			gen->write("\tgoto " + end + "\n");
			gen->write(l + ":" + "\n");
			child[2]->gen_code(gen);
			gen->write(end + ":" + "\n");
		}
	}
};
class Iteration : public Node
{
	Var* v;
	public:
	void check_scope(Scope_stack* scope)
	{
#ifdef DEBUG
cout << "Iteration::check_scope()1" << endl;
#endif
		unsigned size = child.size();
		if( ! scope->search("i") ) //there is no "i" declared
		{
			/*t_entry* t = new t_entry("i");
			t->type = IVAL;
			entry_vec entries = new vector<t_entry*>;
			entries->push_back(t);
			((Block*)child[size-1])->set_func_params(entries);
			*/

			//Declaring a Var to hold 'i'.
			v = new Var("i");
			v->set_line(0);
			((Block*)child[size-1])->declare_var(v);
		}
		for(int i=0; i<size; ++i)
			child[i]->check_scope(scope);
#ifdef DEBUG
cout << "Iteration::check_scope()2" << endl;
#endif
	}
	Iteration(Node* n0) : Node(n0){//forever
		v = NULL;
	}
	Iteration(Node* n0, Node* n1) : Node(n0,n1){ // for var
		v = NULL;
	}
	Iteration(Node* n0, Node* n1, Node* n2, Node* n3) : Node(n0,n1,n2,n3){
		v = NULL;
	}
	void gen_code(Code_gen* gen)
	{
		unsigned size = child.size();
		string l = gen->new_label();
		unsigned reg; // Register number will be ready only
					  // when the block code is generated!

		((Block*)child[size-1])->set_registers(gen);
		if(v)
			reg = v->get_register();
		switch(size)
		{
			case 1:
			{
				if(v){
					gen->write("\tldc 0\n");
					gen->write("\tistore ", reg);
				}
				gen->write(l + ":" + "\n");
				((Block*)child[0])->gen_code(gen);
				if(v){
					gen->write("\t; ++i\n");
					gen->write("\tldc 1\n");
					gen->write("\tiload ", reg);
					gen->write("\tiadd\n");
					gen->write("\tistore ", reg);
				}
				gen->write("goto " + l + "\n");
				break;
			}
			case 2:
			{
				if(v){
					gen->write("\tldc 0\n");
					gen->write("\tistore ", reg);
				}
				else
					cout << "Warning: It is impossible to use this structure "
						 << "in a FOR statement with an \"i\" variable declared.\n";
				string l2 = gen->new_label();

				gen->write(l + ":" + "\n");
				child[0]->gen_code(gen);
				gen->write("\tiload ", reg);
				gen->write("\tisub\n");
				gen->write("\tifle " + l2 + "\n");
				((Block*)child[1])->gen_cmd_code(gen);
				if(v){
					gen->write("\t; ++i\n");
					gen->write("\tldc 1\n");
					gen->write("\tiload ", reg);
					gen->write("\tiadd\n");
					gen->write("\tistore ", reg);
				}
				gen->write("\tgoto " + l + "\n");
				gen->write(l2 + ":" + "\n");
				break;
			}
			case 4:
			{
				string l2 = gen->new_label();
				if(v){
					gen->write("\tldc 0\n");
					gen->write("\tistore ", reg);
				}
				child[0]->gen_code(gen);
				gen->write(l + ":" + "\n");
				child[1]->gen_code(gen);
				gen->write("ifeq " + l2 + "\n");
				((Block*)child[3])->gen_cmd_code(gen);
				gen->write("\t; for(;;exp)\n");
				child[2]->gen_code(gen);

				if(v){
					gen->write("\t; ++i\n");
					gen->write("\tldc 1 ; Increment implicit i.\n");
					gen->write("\tiload ", reg);
					gen->write("\tiadd\n");
					gen->write("\tistore ", reg);
				}
				gen->write("goto " + l + "\n");
				gen->write(l2 + ":" + "\n");
				break;
			}
		}
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
		if(child.size()>0) // Max 1 child
		{
			if(child[0]->get_type() != dt)
				cout << "Warning: Return statement in line "
					 << yylineno <<" should "
					 << "return " << type_str(dt)
					 << " but it is returning "
					 << type_str(child[0]->get_type())
					 << " instead." << endl;
		}
			}
	void check_scope(Scope_stack* scope)
	{
		t_entry* t = scope->search_current_function();
		if(t)
			set_type(t->type);
		if(child.size()>0)
			child[0]->check_scope(scope);
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
	string func_id;
	Arguments* args;
	int line;
	public:
	Function_call(Arguments* arg, string function)
	{
		func_id = function;
		args = arg;
		func = NULL;
	}
	
	void set_line(int l){line = l;}
	int get_line(){return line;}

	void evaluate()
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
	void check_scope(Scope_stack* scope)
	{
		func = scope->search(func_id);
		if(!func)
			cout << "Function " << func_id << "undeclared in this scope\n";
		unsigned size = child.size();
		for(int i=0; i<size; ++i)
			child[i]->check_scope(scope);
	}
	string get_id()
	{
		return func_id;
	}

};

class Print : public Node
{
	public:
	Print(Var* v) : Node(v)
	{}
	void gen_code(Code_gen* gen)
	{
		int reg = ((Var*)child[0])->get_register();
		gen->write("\tiload ");
	   	gen->write(reg);
	   	gen->write("\n");
		gen->write("\tinvokestatic out.print(I)V\n");
	}
};
class Read : public Node
{
	public:
	Read(Var* v) : Node(v)
	{}
	void gen_code(Code_gen* gen)
	{
		int reg = ((Var*)child[0])->get_register();
		gen->write("\tinvokestatic out.read()I\n");
		gen->write("\tistore ", reg);
	}
};

#endif
