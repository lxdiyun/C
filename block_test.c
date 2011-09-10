#include <stdio.h>

int main()
{
   int block_var = 10;
   printf("Init - block_var = %d\n", block_var);

   // Block Test
   {
      int block_var = 9;

      printf("In the block - block_var = %d\n", block_var);
   }

   printf("After block - block_var = %d\n", block_var);

   return 0;
}
