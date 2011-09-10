#include <signal.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc,char **argv) {
	double y;
	sigset_t intmask;
	int i,repeat_factor;

	if(argc != 2) {
		fprintf(stderr,"Usage:%s repeat_factor\n\a",argv[0]);
		exit(1);
	}

	if((repeat_factor = atoi(argv[1])) < 1)
		repeat_factor = 10;
	sigemptyset(&intmask);// 将信号集合设置为空
	sigaddset(&intmask,SIGINT);// 加入中断Ctrl+C信号
	while(1) {
		// 阻塞信号,我们不希望保存原来的集合所以参数为NULL
		sigprocmask(SIG_BLOCK,&intmask,NULL);
		fprintf(stderr,"SIGINT signal blocked\n");
		for(i = 0; i < repeat_factor; ++i)
			y = sin((double)i);
		fprintf(stderr,"Blocked calculation is finished\n");
		// 取消阻塞
		sigprocmask(SIG_UNBLOCK,&intmask,NULL);
		fprintf(stderr,"SIGINT signal unblocked\n");
		for(i = 0; i < repeat_factor; ++i)
			y = sin((double)i);
		fprintf(stderr,"Unblocked calculation is finished\n");
	}
	exit(0);
}
