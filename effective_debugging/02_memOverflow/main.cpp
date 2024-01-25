#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *CopyString(char *s)
{
    char *newString = (char *)malloc(strlen(s));
    strcpy(newString, s);
    return newString;
}

#define N 6
int main()
{
    int *p = (int *)malloc(N*sizeof(int));
    for (int i = 0; i <= N; i++)
        p[i] = 0;
    return 0;
}
