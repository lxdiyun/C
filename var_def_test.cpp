#include <stdio.h>

class counter
{
   public:
      static int get_create_time() {return m_create_time;};

      counter() { ++m_create_time;};

   private:
      static int m_create_time; 

};

int counter::m_create_time = 0;

int main()
{
   int def_at_begin = 0;

   printf("def_at_begin = %d\n", def_at_begin);

   //   printf("def_at_middle = %d\n", def_at_middle);

   int def_at_middle = 1;

   printf("def_at_middle = %d\n", def_at_middle);

   if(1)
   {
      int def_in_block = 4;

      printf("def_in_block = %d\n", def_in_block);
   }

   int def_at_end = 2;

   printf("def_at_end = %d\n", def_at_end);


   int loop_count;

   for(loop_count = 0; loop_count < 5; ++ loop_count)
   {
      const counter tempCounter;
   }

   printf("Counter create time after loop = %d\n", loop_count);

   return 0;
}
