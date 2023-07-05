#include <iostream>
#include <string>
#include <chrono>
#include <cstdio>

#ifndef __TIMER_LOG

#define __TIMER_LOG(str) \
for (struct { bool done; std::chrono::high_resolution_clock::time_point start; } timer = { false, std::chrono::high_resolution_clock::now() }; \
timer.done == false ? (timer.done = true) : false; \
printf(str "%.6f seconds\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - timer.start).count() / double(1000000)))

#endif

#define SIZE_NAME 10000
#define LOOP_NUM 100000

void foo()
{
    for (int i= 0; i < LOOP_NUM; i++)
    {
        std::string name;
        name.reserve(SIZE_NAME);
        size_t pos = 0;
        while (pos < SIZE_NAME)
        {
            name.append(1, '\\');
            pos++;
        }
    }
}

void foo2()
{
    for (int i= 0; i < LOOP_NUM; i++)
    {
        std::string name;
        name.resize(SIZE_NAME);
        size_t pos = 0;
        while (pos < SIZE_NAME)
        {
            name[pos++] = '\\';
        }
    }
}

int main(int argc ,char **argv)
{
    __TIMER_LOG("foo time: ") {
        foo();
    }

    __TIMER_LOG("foo2 time: ") {
        foo2();
    }
    return 0;
}
