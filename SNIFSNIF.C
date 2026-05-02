#include <stdio.h>


int main()
{
	char *a;

	a = 0xdc00;
	while (1==1)
	{
		printf ("%c",*a);
		a=a+1;
		if (a==0xdc07)
		a = 0xdc00;
	}
	return 0;
}
