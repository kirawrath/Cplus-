#ifndef CODE_GEN_CPP
#define CODE_GEN_CPP
#include "../include/code_gen.h"
#include "../include/parse_tree/node.h"
using namespace std;

Code_gen::Code_gen(Node* root_node)
{
	root = root_node;
}
void Code_gen::generate_file(string name)
{
	ofstream outfile(name.c_str(), ios::out);

	outfile << ".class public out \n";
	outfile << "	.super java/lang/Object \n";
	outfile << "	.method public <init>()V \n";
	outfile << "	aload_0 \n";
	outfile << "	invokespecial java/lang/Object/<init>()V \n";
	outfile << "	return \n";
	outfile << "	.end method \n";


	outfile << "	.method public static print(I)V\n";
	outfile << "	.limit locals 3\n";
	outfile << "	.limit stack 3\n";

	outfile << "	iload 0\n";
	outfile << "	getstatic java/lang/System/out Ljava/io/PrintStream; \n";
	outfile << "	swap \n";
	outfile << "	invokevirtual java/io/PrintStream/println(I)V   ; print x\n";

	outfile << "	return\n";
	outfile << "	.end method\n";
	outfile << "	.method public static main([Ljava/lang/String;)V \n";
	outfile << "	.limit stack 50 \n";
	outfile << "	.limit locals 100 \n";

	int count = 0;
	root->gen_code(outfile, count);


	outfile << "	invokestatic simple.print(I)V\n";
	outfile << "	return        ; return from main\n";
	outfile << ".end method\n";
}

#endif
