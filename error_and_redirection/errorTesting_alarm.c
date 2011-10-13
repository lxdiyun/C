#include <stdio.h>

int main(int argc, char const* argv[])
{
	alarm(2);

	unsigned int index;

	for (index = 0; 3 > index; ++index) 
	{
		printf("sleep %d second\n", 3 - index);
		sleep(1);
	}
	
	return 0;
}
