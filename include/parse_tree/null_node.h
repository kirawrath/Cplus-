#ifndef NULL_NODE
#define NULL_NODE

#include "node.h"
class Null_node : public Node
{
	public:
	Null_node() : Node(){}
	data_type get_type()
	{
		return NOTYPE;
	}
	bool is_null()
	{
		return true;
	}
	t_entry* get_entry()
	{
		cout << "Null node get_entry requested!" << endl;
	}
};
#endif
