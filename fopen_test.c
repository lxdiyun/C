#include <stdio.h>
#include <stdlib.h>

int main()
{
   FILE * helloFile = fopen("./hello.txt", "rb");

   fputs("hello abcdefghiljklmnopqrstuvwxyz\n", helloFile);

   fclose(helloFile);

   const int BUF_SIZE = 100;
   char * lineBuf = calloc(BUF_SIZE, sizeof(char));
   size_t bufLength = BUF_SIZE;

   helloFile = fopen("./hello.txt", "r");

   getline(&lineBuf, &bufLength, helloFile);

   printf("r:%s\n", lineBuf);

   fclose(helloFile);

   helloFile = fopen("./hello.txt", "rb");

   getline(&lineBuf, &bufLength, helloFile);

   printf("rb:%s\n", lineBuf);

   fclose(helloFile);

   free(lineBuf);

   return 0;
}

