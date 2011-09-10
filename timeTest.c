#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void function() {
	unsigned int i,j;
	double y;
	for(i = 0;i < 1000; ++i)
		for(j = 0; j < 1000; ++j)
			y = sin((double)i);
}

int main() {
	struct timeval tpstart,tpend;
	float timeuse;
	
	gettimeofday(&tpstart,NULL);
	function();
	gettimeofday(&tpend,NULL);
	timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec)
			+ tpend.tv_usec - tpstart.tv_usec;
	timeuse /= 1000000;
	printf("Used Time:%f\n",timeuse);
	exit(0);
}

