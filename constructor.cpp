#include "constructor.h"

Constructor::Constructor()
{
   this->m_i = 1;
}
Constructor::Constructor(int i)
{
   Constructor();
   if(i != 0)
   {
      this->m_i = i;
   }

   cout << this->m_i << endl;
}

int main()
{
   Constructor constructor1(1);
   Constructor constructor0(0);

   return 0;
}
