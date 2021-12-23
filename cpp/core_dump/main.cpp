#include <iostream>

void crash(int i)
{
   std::cout << i << std::endl;
   int arr[2];
   arr[5] = 10;
   return;
}

int main(void)
{
   std::cout << "crash" << std::endl;
   crash(1);
   std::cout << "end" << std::endl;
   return 0;
}