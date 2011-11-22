#ifndef CODE_GEN_H
#define CODE_GEN_H
#include "../include/parse_tree/node.h"
#include <iostream>
#include <fstream>
using namespace std;

class Code_gen
{
	Node* root;
	int reg_counter;
	ofstream* file;
	public:
	Code_gen(Node* root_node);
	void generate_file(string name = "out.j");
	unsigned get_counter();
	void inc_counter();
	string new_label();
	void write(string);
	void write(string, string);
	void write(int);
	void write(string,int);
};

#endif
