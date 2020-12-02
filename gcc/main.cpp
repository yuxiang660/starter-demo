#include <stdio.h>
#include "./shared_lib/bar.h"
#include "./shared_lib/foo.h"
 
int main(void)
{
   printf("This is a shared library test...\n");
   say_bar();
   say_foo();
   return 0;
}
