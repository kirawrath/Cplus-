#ifndef NODE
#define NODE
#include<iostream>
#include<vector>
#include "../symbol_table.h"
using namespace std;
typedef vector<t_entry*>* entry_vec;

class Scope_stack;
class Node
{
	public:
	virtual void set_entry(t_entry* t){cout<< "BOOYA0"<<endl;}
	virtual t_entry* get_entry(){cout<< "BOOYA1"<<endl;}
	virtual string get_id(){cout<< "BOOYA2"<<endl;}
	virtual data_type get_type(){cout<< "BOOYA3"<<endl;} 
	virtual void set_type(data_type dt){
		cout<< "BOOYA4"<<endl;}
	virtual int get_line(){cout<< "BOOYA5"<<endl;}
	virtual void set_indexer(int){cout<< "BOOYA6"<<endl;} //for arrays
	virtual void set_indexer(Node*){cout<< "BOOYA7"<<endl;} //for arrays
	virtual bool is_null(){return false;}
	virtual void check_scope(Scope_stack* s)
	{
#ifdef DEBUG
		cout << "Checking scope..." << endl;
#endif
		unsigned size = child.size();
		for(int i=0; i<size; ++i)
			child[i]->check_scope(s);
#ifdef DEBUG
		cout << "Scope checked!" << endl;
#endif
	}
	
	vector<Node*> child;
	virtual void evaluate()
	{
		unsigned size = child.size();
		try{
			for(int i=0; i<size; ++i) //Removing NULL childs
				if(!child[i])
					child.erase(child.begin() + i);

			for(int i=0; i<size; ++i)
				child[i]->evaluate();
		}
		catch(string problem){
			cout<<problem<<endl;
		}
		catch(const char* problem){
			cout << problem << endl;
		}
	}

	Node* get_child(int i)
	{
		return child[i];
	}
	// Now a lot of overloads!
	Node(){}
	Node(Node* c1)
	{
		add_child(c1);
	}
	Node(Node* c1, Node* c2)
	{
		add_child(c1);
		add_child(c2);
	}
	Node(Node* c1, Node* c2, Node* c3)
	{
		add_child(c1);
		add_child(c2);
		add_child(c3);
	}
	Node(Node* c1, Node* c2, Node* c3, Node* c4)
	{
		add_child(c1);
		add_child(c2);
		add_child(c3);
		add_child(c4);
	}
	void print_id_inorder()
	{//for binary trees
		unsigned n = child.size();
		if(n>0)
			child[0]->print_id_inorder();
		if(n>1)
			child[1]->print_id_inorder();
	}
	void print_id_postorder()
	{
		unsigned n = child.size();
		for(int i=0; i<n; ++i)
			child[i]->print_id_postorder();
		//cout<<id<<endl;
	}
	void print_id_preorder()
	{
		unsigned n = child.size();
		for(int i=0; i<n; ++i)
			if(child[i])
				child[i]->print_id_preorder();
			else
				cout << "Null child!" << endl;
	}

	void add_child(Node* c)
	{
		child.push_back(c);
	}
	~Node()
	{
		unsigned n = child.size();
		for(int i=0; i<n; ++i)
			delete child[i];
		delete this;
	}
};
#endif
