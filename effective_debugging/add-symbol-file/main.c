#include "mm_symbol.h"
#include <stdio.h>

int main()
{
#ifdef NDEBUG
   printf("define NDEBUG=%d\n", NDEBUG);
#else
   printf("not define NDEBUG\n");
   printf("not define NDEBUG\n");
   printf("not define NDEBUG\n");
#endif

   init();

   for (int i = 0; i < 2; i++)
   {
      printf("g_blocks[%d]=%p, g_blocks[%d].size=%d, g_blocks[%d].prev=%p, g_blocks[%d].next=%p\n",
         i, &g_blocks[i], i, g_blocks[i].size, i, g_blocks[i].prev, i, g_blocks[i].next);
   }

   return 0;
}
