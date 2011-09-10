/*******************************************************************
 * twordcount.c - thread word counter for two files
 * condition variable allows counter fuctions 
 * 	to report result early
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

struct arg_set {				// two values in one arg
	char *fname;				// file to examine
	int count;				// number of words
	int thread_id;				// thread id
};

struct arg_set *mailbox;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;

int main(int argc,char *argv[]) {
	pthread_t t1,t2,t3;
	struct arg_set args1,args2,args3;	// two threads
	void *count_words(void *);		// two argsets
	int reports_in = 0;
	int total_words = 0;

	if(argc != 4) {
		fprintf(stderr,"Usage: %s file1 file2 file3\n",argv[0]);
		exit(1);
	}

	pthread_mutex_lock(&lock);		// lock the report box now
	
	args1.fname = argv[1];
	args1.count = 0;
	args1.thread_id = 1;
	pthread_create(&t1,NULL,count_words,(void *)&args1);

	args2.fname = argv[2];
	args2.count = 0;
	args2.thread_id = 2;
	pthread_create(&t2,NULL,count_words,(void *)&args2);

	args3.fname = argv[3];
	args3.count = 0;
	args3.thread_id = 3;
	pthread_create(&t3,NULL,count_words,(void *)&args3);

	while(reports_in < 3) {
		printf("MAIN: waiting for flag to go up\n");
		pthread_cond_wait(&flag,&lock);	// wait for notify
		printf("MAIN: Wow!flag was raised, I have the lock\n");
		printf("%7d: %s\n",mailbox->count,mailbox->fname);
		total_words += mailbox->count;
		
		if(mailbox == &args1)
			pthread_join(t1,NULL);
		if(mailbox == &args2)
			pthread_join(t2,NULL);
		if(mailbox == &args3)
			pthread_join(t3,NULL);
		mailbox = NULL;
		pthread_cond_signal(&flag);
		++reports_in;
	}
	printf("%7d: total word\n",total_words);

	return 0;
}

void *count_words(void *a) {
	struct arg_set *args = a;		// casts arg back to correct type
	FILE *fp;
	int c,id = args->thread_id,prevc = '\0';

	if((fp = fopen(args->fname,"r")) != NULL) {
		while((c = getc(fp)) != EOF) {
			if(!isalnum(c) && isalnum(prevc))
				++args->count;
			prevc = c;
		}
		fclose(fp);
	} else
		perror(args->fname);
	printf("COUNT%d: waiting to get lock\n",id);
	pthread_mutex_lock(&lock);		// get the mailbox
	printf("COUNT%d: have lock,storing data\n",id);
	while(mailbox != NULL) {		// if the mailbox is not empty,wait
		pthread_mutex_unlock(&lock);
		pthread_mutex_lock(&lock);
		// pthread_cond_wait(&flag,&lock);
	}
	mailbox = args;				// put ptr to our args there
	printf("COUNT%d: rasing flag\n",id);
	pthread_cond_signal(&flag);		// raise the flag
	printf("COUNT%d: unlocking box\n",id);
	pthread_mutex_unlock(&lock);		// release the mail box

	return NULL;
}
