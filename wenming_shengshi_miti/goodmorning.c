#include <stdio.h>

int main(int argc, char *argv[])
{
   int count;
   FILE* iFile = fopen(argv[1], "r");
   
   while (EOF != fscanf(iFile, "%d\n", &count))
   {
      int i;

      for (i = 0; i < count; ++i)
      {
         int mod = i % 24;
         if ((6 > mod) || (8 < mod))
         {
            printf("hello\n");
         }
         else
         {
            printf("Good Morning\n");
         }
      }
   }

   fclose(iFile);

   return 0;
}
