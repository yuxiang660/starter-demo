#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

std::mutex gMutex;

int Reenter()
{
   std::lock_guard<std::mutex> lLock(gMutex);
   return 10;
}

int Callback()
{
   std::lock_guard<std::mutex> lLock(gMutex);
   return Reenter();
}

int entry()
{
   std::cout << "Entry...." << std::endl;
   int ret = Callback();
   return ret;
}

int main(int argc, char **argv)
{
   // Prints hello message...
   std::cout << "Hello CMake World!" << std::endl;
   std::thread lThread(entry);
   lThread.join();
   std::cout << "sub thread is gone!" << std::endl;
   return 0;
}
