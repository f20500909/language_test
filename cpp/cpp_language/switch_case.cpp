#include <iostream>
using namespace std;
 
int main ()
{
   // 局部变量声明
   char grade = 'A';
 
   switch(grade)
   {
   case 'A' :
      cout << "A" << endl; 
   case 'B' :
   case 'C' :
      cout << "C" << endl;
   case 'D' :
      cout << "D" << endl; break;
   case 'F' :
      cout << "F" << endl;
   default :
      cout << "defalut" << endl;
   }
   cout << "done : " << grade << endl;
 
   return 0;
}