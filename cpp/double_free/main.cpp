#include <iostream>
#include <cstdio>

int main()
{
   // pointer demo to FILE
   FILE *demo;

   // Creates a file "demo_file"
   // with file access as write-plus mode
   demo = fopen("demo.log", "w+");

   // adds content to the file
   fprintf(demo, "%s %s %s", "Welcome", "to",
           "GeeksforGeeks");

   // closes the file pointed by demo
   fclose(demo);
   fclose(demo);

   return 0;
}
