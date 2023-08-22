#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <unordered_map>


int main(int argc ,char **argv)
{
    std::cout << "Test vscode conditional breakpoint" << std::endl;

    for (int i = 0; i < 10; i++)
        printf("i=%d\n", i);

    std::unordered_map<int, uint64_t> ids;
    ids[1] = 55;
    ids[4] = 100;
    for (const auto &[k, v] : ids)
    {
        int k2 = k;
        printf("k2=%d\n", k2);
        printf("v=%ju\n", v);
    }

    for (const auto &iter : ids)
    {
        int k = iter.first;
        int v = iter.second;
        printf("k1=%d\n", k);
        printf("v1=%ju\n", v);
    }

    return 0;
}
