#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CreateInstance()
{
    printf("CreateInstance\n");
}

const char *STR = "vmredh.open.com";

int main()
{
    const char *str = STR;
    printf("%s\n", str);
    auto p = CreateInstance;
    p();

    return 0;
}
