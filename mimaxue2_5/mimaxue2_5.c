#include <stdio.h>

int main(int argc,char *argv[]) {
	int arrayA[] = {1,1,1,0,1,0,0};
	int arrayB[] = {0,0,1,0,1,1,0,1,1,0,1,1,0,0,0};
	int i,result = 0;
	
	for(i = 0; i < 105; ++i) {
		if(i % 5 ==0 && i != 0)
			printf(" ");
		if(i % 20 ==0 && i != 0) 
			printf("\n");
		result = ((!((arrayA[i % 7] + arrayB[i % 15]) % 2)) * result 
			+ arrayA[i % 7]) % 2;
		printf("%d",result);
	}
	
	return 0;
}
