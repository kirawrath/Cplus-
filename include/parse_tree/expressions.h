#ifndef BINARY_OP
#define BINARY_OP

#include "node.h"
class Expression : public Node
{// Should not be instantiated

	protected:
	data_type type;
	public: 
	data_type get_type()
	{
		evaluate();
#ifdef DEBUG
		cout << "Expression type: "<< type << endl;
		cout << "Expression csize: "<< child.size() << endl;
#endif
		return type;
	}
	void set_type(data_type dtt)
	{
		type = dtt;
	}
};


class Ternary_operator : public Expression
{
	public:
	Ternary_operator(Node* n0, Node* n1, Node* n2)
	{
		add_child(n0); add_child(n1); add_child(n2);
	}
	void evaluate()
	{
		data_type d0, d1;
		d0 = child[1]->get_type();
		d1 = child[2]->get_type();
		if(d0!=d1)
			throw "True and False value in ternary operator should be both of the same type";
		type = d0;
	}
	data_type get_type()
	{
		evaluate();
		return type;
	}		
};

class Relational_expression : public Expression
{
	int op;
	public:
	Relational_expression(Expression* s0, Expression* s1, int op)
	{
		add_child(s0); add_child(s1);
		this->op = op;
	}
	data_type get_type()
	{
		return IVAL; // Boolean is int.
	}
};
class Aritimetic_expression : public Expression
{// sum_exp
	public:
	Aritimetic_expression(Expression* left, Expression* right)
	{
		add_child(left); add_child(right);
	}
	void evaluate()
	{
		data_type d0, d1;
		d0 = child[0]->get_type();
		if(child[1])
			d1 = child[1]->get_type();
		else
			cout << "Null in Arithmetic!"<< endl;
#ifdef DEBUG
		cout << "Arithmetic_expression: ";
		cout << d0 << " " << d1 << endl;
#endif
		if(d0 != d1)
			throw "Different types in arithmetic expression";

		set_type(d0);
	}
};
class Sum : public Aritimetic_expression
{
	public:
	Sum(Expression* left, Expression* right) : Aritimetic_expression(left, right){}

	void gen_code(Code_gen* gen)
	{
		child_gen_code(gen);
		gen->write("\tiadd\n");
	}
};

class Multiplication : public Aritimetic_expression
{
	public:
	Multiplication(Expression* left, Expression* right) : Aritimetic_expression(left, right){}
	void gen_code(Code_gen* gen)
	{
		child_gen_code(gen);
		gen->write("\timul\n");
	}
};
class Subtraction : public Aritimetic_expression
{
	public:
	Subtraction(Expression* left, Expression* right) : Aritimetic_expression(left, right){}
	void gen_code(Code_gen* gen)
	{
		child_gen_code(gen);
		gen->write("\tisub\n");
	}
};
class Division : public Aritimetic_expression
{
	public:
	Division(Expression* left, Expression* right) : Aritimetic_expression(left, right){}
	void gen_code(Code_gen* gen)
	{
		child_gen_code(gen);
		gen->write("\tidiv\n");
	}
};

class Factor : public Expression
{
	public:
	Factor(Node* n)
	{
		add_child(n);
	}
	data_type get_type()
	{
#ifdef DEBUG
		cout << "Factor type: " << child[0]->get_type() << endl;
#endif
		return child[0]->get_type();
	}
	void evaluate()
	{
		child[0]->evaluate();
	}
	void check_scope(Scope_stack* scope)
	{
		child[0]->check_scope(scope);
	}
};
class Factorial : public Expression
{
	public:
	Factorial(Node* n)
	{
		add_child(n);
	}
	data_type get_type()
	{
		return child[0]->get_type();
	}
	void evaluate()
	{
		data_type dt = child[0]->get_type();
		if(dt != IVAL && dt != LVAL)
			throw "Incompatible type for factorial";
	}
};
#endif
