#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>

int main() {
	unsigned int i;

	alarm(1);

	for(i = 0; 1; ++i)
		printf("I=%d\n",i);
	exit(1);
}
