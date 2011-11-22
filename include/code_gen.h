#ifndef CODE_GEN
#define CODE_GEN
#include "../include/parse_tree/node.h"
#include <fstream>
using namespace std;

class Code_gen
{
	Node* root;
	public:
	Code_gen(Node* root_node);
	void generate_file(string name = "out.j");
};

#endif
