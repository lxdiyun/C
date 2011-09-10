#include <iostream>

using namespace std;

class testclass2
{
public:
   testclass2(int i)
   {
      i_m = i;

      cout << i << endl;
   }
   int getI()
   {
      return i_m;
   }
private:
   int i_m;
};

class TestClass
{
private:
   static testclass2 class2;
public:
   static void testClassInit()
   {
      cout << "Begin Init" << endl;
      cout << "class2's i=" << class2.getI() << endl;
   };

   static void fun1()
   {
      //cout << "class2's i=" << class2.getI() << endl;
   }
};

 testclass2 TestClass::class2(2);

typedef enum 
{
   enum0 = 0,
   enum1 = 1
} enumType;

int main()
{
   TestClass::testClassInit();
   TestClass::fun1();

   char i = 3;

   enumType enumTest = static_cast<enumType>(i);

   cout << static_cast<int>(enumTest) << endl;

   return 0;
}
