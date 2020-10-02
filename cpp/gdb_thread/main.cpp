#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

std::mutex _mutex;
static int total = 0;

void fun(int num)
{
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::lock_guard<std::mutex>lock(_mutex);
        total += num;
        std::cout << "num: " << num << std::endl;
        std::cout << "total: " << total << std::endl;
    }
}

int main(int argc ,char **argv)
{
    std::cout << "GDB Muti-thread Debugging" << std::endl;

    std::thread t1(fun, 3);
    std::thread t2(fun, 5);
    t1.join();
    t2.join();

    return 0;
}
