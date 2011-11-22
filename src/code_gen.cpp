#ifndef CODE_GEN_CPP
#define CODE_GEN_CPP
#include "../include/code_gen.h"
#include "../include/parse_tree/node.h"
#include <iostream>
#include <fstream>
using namespace std;

Code_gen::Code_gen(Node* root_node)
{
	root = root_node;
}
void Code_gen::generate_file(string name)
{
	file = new ofstream (name.c_str(), ios::out);
	if(file->fail())
		cout << "Fail file!" << endl;

	(*file) << ".class public out " << endl;
	(*file) << "	.super java/lang/Object " << endl;
	(*file) << "	.method public <init>()V " << endl;
	(*file) << "	aload_0 " << endl;
	(*file) << "	invokespecial java/lang/Object/<init>()V " << endl;
	(*file) << "	return " << endl;
	(*file) << "	.end method " << endl;
	/*********** PRINT ************/
	(*file) << "	.method public static print(I)V" << endl;
	(*file) << "	.limit locals 3" << endl;
	(*file) << "	.limit stack 3" << endl;
	(*file) << "	iload 0" << endl;
	(*file) << "	getstatic java/lang/System/out Ljava/io/PrintStream; " << endl;
	(*file) << "	swap " << endl;
	(*file) << "	invokevirtual java/io/PrintStream/println(I)V   ; print x" << endl;
	(*file) << "	return" << endl;
	(*file) << "	.end method" << endl;


	/*********** READ ************/
	(*file) << ".method public static read()I" << endl;
	(*file) << "	.limit locals 10 " << endl;
	(*file) << "	.limit stack 10 " << endl;
	(*file) << "	ldc 0 " << endl;
	(*file) << "	istore 1  ; this will hold our final integer " << endl;
	(*file) << "	Label1: " << endl;
	(*file) << "	getstatic java/lang/System/in Ljava/io/InputStream; " << endl;
	(*file) << "invokevirtual java/io/InputStream/read()I " << endl;
	(*file) << "	istore 2 " << endl;
	(*file) << "	iload 2 " << endl;
	(*file) << "	ldc 10   ; the newline delimiter " << endl;
	(*file) << "	isub " << endl;
	(*file) << "	ifeq Label2 " << endl;
	(*file) << "	iload 2 " << endl;
	(*file) << "	ldc 32   ; the space delimiter " << endl;
	(*file) << "	isub " << endl;
	(*file) << "	ifeq Label2 " << endl;
	(*file) << "	iload 2 " << endl;
	(*file) << "	ldc 48   ; we have our digit in ASCII, have to subtract it from 48 " << endl;
	(*file) << "	isub " << endl;
	(*file) << "	ldc 10 " << endl;
	(*file) << "	iload 1 " << endl;
	(*file) << "	imul " << endl;
	(*file) << "	iadd " << endl;
	(*file) << "	istore 1 " << endl;
	(*file) << "	goto Label1 " << endl;
	(*file) << "	Label2: " << endl;
	(*file) << "	;when we come here we have our integer computed in Local Variable 1 " << endl;
	(*file) << "	iload 1 " << endl;
	(*file) << "	ireturn " << endl;
	(*file) << "	.end method " << endl;

	/*******************************************************/

	reg_counter = 0;
	root->gen_code(this);

}
unsigned Code_gen::get_counter()
{
	return reg_counter;
}
void Code_gen::inc_counter()
{
	reg_counter++;
}

void Code_gen::write(string str)
{
	(*file) << str;
}
void Code_gen::write(string str1, string str2)
{
	(*file) << str1 << str2 << endl;
}
void Code_gen::write(int value)
{
	(*file) << value;
}
void Code_gen::write(string str, int value)
{
	(*file) << str << value << endl;
}
string Code_gen::new_label()
{
	static string s="A";
	int size = s.size();
	if(s[size-1] == 'Z')
	{
		bool done = false;
		for(int i=size-2; i>=0; --i)
		{
			if(s[i] != 'Z')
			{
				s[i]++;
				for(int j=i+1; j<size; ++j)
					s[j] = 'A';
				done = true;
			}
			if(done)
				break;
		}
		if(!done)
		{
			for(int i=0; i<size; ++i)
				s[i] = 'A';
			s+='A';
		}
	}
	else
		s[size-1]++;
	return s;
}
#endif
