#include <stdio.h>

#define G_MAXNUM 99999

int main()
{	
	long stickNum;
	long answer[G_MAXNUM];
	long stick;
			
	while(1)
	{
		long maxstick = 1;
		long total = 0;
		
		scanf("%ld", &stickNum);
		
		if (stickNum == 0)
			break;
		
		for (long i = 0; i < G_MAXNUM; ++i)
			answer[i] = 0;
		
		for (long j = 0; j < stickNum; ++j) 
		{
			scanf("%ld", &stick);
			
			if (stick > maxstick)
				maxstick = stick;
			
			total += stick;			
		}
		
		for (long k = maxstick; k < G_MAXNUM; ++k)
		{
			if (!answer[k] && total % k == 0) {
				printf("%ld\n", k);
				break;
			}
			else if(!answer[k])
			{
				for (long l = k; l < G_MAXNUM; l += k)
					answer[l] = 1;
			}
		}
	}
	return 0;
}
