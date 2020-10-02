#include <iostream>
#include <string>
#include <algorithm>
#include <string.h>

//__attribute__((no_sanitize_address))
void func(char* pbm_loc) 
{
    std::string loc_str(pbm_loc, 8);
    for (int i = 0; i < 8; i++)
    {
        printf("pbm_loc[%d] = %d, loc_str[%d] = %d\n", i, pbm_loc[i], i, loc_str[i]);
    }
}

//__attribute__((no_sanitize_address))
void func2(char* pbm_loc)
{
    std::string loc_str2(8, '\0');
    memcpy(&loc_str2[0], pbm_loc, 8);
    for (int i = 0; i < 8; i++)
    {
        printf("pbm_loc[%d] = %d, loc_str2[%d] = %d\n", i, pbm_loc[i], i, loc_str2[i]);
    }
}

int main(int argc ,char **argv)
{
    std::cout << "Gcc Address Sanitizer Test" << std::endl;
    uint64_t val = 0b10010000000000000000000000000000;
    char* pbm_loc = (char *)&val;
    func(pbm_loc);
    func2(pbm_loc);
    return 0;
}
