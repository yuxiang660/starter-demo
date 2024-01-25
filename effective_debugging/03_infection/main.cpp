#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void AccessFree()
{
    long* lArray = new long[10];
    delete lArray;

    lArray[1] = 1;
    lArray[0] = 2;
}

void Victim()
{
    void* p = malloc(32);
    free(p);
}

int main()
{
    void* p = malloc(128);
    char* str = strdup("a string");
    free(p);
    AccessFree();
    Victim();
    return 0;
}
