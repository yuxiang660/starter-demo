#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <stdio.h>

int main()
{
   std::mutex m1;
   std::mutex m2;
   std::thread t1([&m1, &m2]
                  {
                     printf("1. Acquiring m1.");
                     m1.lock();
                     std::this_thread::sleep_for(std::chrono::milliseconds(10));
                     printf("1. Acquiring m2");
                     m2.lock();
                  });
   std::thread t2([&m1, &m2]
                  {
                     printf("2. Acquiring m2");
                     m2.lock();
                     std::this_thread::sleep_for(std::chrono::milliseconds(10));
                     printf("2. Acquiring m1");
                     m1.lock();
                  });

   t1.join();
   t2.join();

   return 0;
}
