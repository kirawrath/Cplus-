int f()
{
	int x;
	read x;
	x = x*x;
	return x;
}
int main()
{
	int x, y, z;

	z = y = x = 10 + 20 + 12;
	x = 0;
	if(x)
		print x;
	else
	{
		x = 666;
		print z;
		print x;
	}

	for(x=3; x < 10; x++)
		print x;

	x = 5;
	for(x)
	{
		print i;
	}

	y = 5!;
	print y;

	z = (y>200) ? 3 : 4;
	print z;


	z = f();
	print z;

}
