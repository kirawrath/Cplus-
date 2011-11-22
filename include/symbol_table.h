#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <iostream>
#include <vector>
//#include "parse_tree/declarations.h"
class Parameter_list;
class Node;
class Null_node;

using namespace std;
typedef enum op_types
{
	DECR, INCR, OITOKR,
	DECL, INCL, OITOKL
}op_type;
enum data_type
{
	LVAL, CVAL, IVAL,
	IARRAY, LARRAY, CARRAY,
	NOTYPE=-1 //Null_node type
};
typedef struct table_entry
{
	std::string ID;
	data_type type;
	unsigned line;
	Node* node;
	Parameter_list* params;
	unsigned reg; // Register
	union{
		long long lval;
		char cval;
		int ival;
	};

	table_entry()
	{
		params = NULL;
	}
	table_entry(string s)
	{
		ID = s;
		params = NULL;
	}
} t_entry;

// Convert the enum to a string
string type_str(data_type);

class Symbol_table
{
	vector<t_entry*> entries;
	public:
	Symbol_table();

	t_entry* insert(t_entry* t);
	t_entry* get_entry(string id);
	Node* get_node(string id);

	void remove(t_entry*);
	void remove(string id);
};


#endif
