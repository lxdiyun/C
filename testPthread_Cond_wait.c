#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int Number = 0;
pthread_mutex_t NMutex;
pthread_cond_t NCond;

void *thread1(void *arg)
{
   struct timespec waiting_time; 
   int cond_wait_count = 0;
   int cond_wait_return = 0;

   sleep(2);

   pthread_mutex_lock(&NMutex);
   printf("Thread get lock\n");

   do
   {
      waiting_time.tv_sec = time(NULL) + 15;


      cond_wait_return = pthread_cond_timedwait(&NCond, &NMutex, &waiting_time);
//      cond_wait_return = pthread_cond_wait(&NCond, &NMutex);
      ++cond_wait_count;

      if (cond_wait_return == 0)
      {
         printf("cond_wait_return:%d\n", cond_wait_return);
      }
   }while (Number == 0 );

   Number = 0 ;
   printf("Number Change Thread:%d - cond_wait_count:%d\n", Number, cond_wait_count);

   pthread_mutex_unlock(&NMutex);
   return NULL;
}

int main(int argc, char* argv[])
{
   pthread_mutex_init(&NMutex, NULL);
   pthread_cond_init(&NCond, NULL);
   pthread_t p1;

   printf("Number Init:%d\n", Number);

   pthread_create(&p1, NULL, thread1, NULL);

   sleep(5);

   pthread_mutex_lock(&NMutex);

   Number = 1;

   printf("Number Change Main:%d\n", Number);

   pthread_mutex_unlock(&NMutex);
   pthread_cond_signal(&NCond);
   printf("Signal Cond, wait for thread\n");

   while (Number == 1)
   {
      sleep(1);
   }

   pthread_join(p1, NULL);

   printf("Number End:%d\n", Number);

   pthread_exit(NULL);

   return 0;
}

