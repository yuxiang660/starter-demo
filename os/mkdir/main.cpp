#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
   const char *logFolder = "./tmp";
   mkdir(logFolder, 0777);
   if (access(logFolder, W_OK) != 0)
   {
      printf("Cannot access to log folder '%s' for GateInfoDBBuilder logs\n", logFolder);
      return -1;
   }

   printf("done\n");
   return 0;
}
