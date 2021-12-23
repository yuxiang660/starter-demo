#include <iostream>
#include <stdio.h>

int main()
{
   std::cout << "/dev/null open test" << std::endl;

   //const char* filename = "test.log";
   const char* filename = "/dev/null";
   FILE *fp = fopen(filename, "w+");
   if (fp == nullptr)
   {
      std::cout << "[ERROR] Fail to open file: " << filename << std::endl;
      return 1;
   }

   fprintf(fp, "%s %s %s", "Welcome", "to", "GeeksforGeeks");
   fclose(fp);

   std::cout << "[DEBUG] Write to the file Done" << std::endl;
   return 0;
}
