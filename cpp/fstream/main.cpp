#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <fstream>

void writeOnfile()
{
   std::ofstream myfile("text.log", std::fstream::app);
   myfile << "aaa\n";
}

int main()
{
   std::ofstream out;

   std::cout << "is_open: " << out.is_open() << std::endl;
   std::cout << "good: " << out.good() << std::endl;
   std::cout << "eof: " << out.eof() << std::endl;
   std::cout << "fail: " << out.fail() << std::endl;
   std::cout << "bad: " << out.bad() << std::endl;

   std::cout << "\n";

   out.open("test.log");
   std::cout << "is_open: " << out.is_open() << std::endl;
   std::cout << "good: " << out.good() << std::endl;
   std::cout << "eof: " << out.eof() << std::endl;
   std::cout << "fail: " << out.fail() << std::endl;
   std::cout << "bad: " << out.bad() << std::endl;

   writeOnfile();

   return 0;
}
