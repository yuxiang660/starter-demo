#include <iostream>
#include <chrono>
#include <thread>
#include <string>

/*
Question: How to use GDB attach the prog for debugging?
Answer:
    1. make
        make the debug version: main
    2. ./main
        run the test, the test will run in an infinite loop by adding one to i
    3. go to another terminal
    4. ps -u | grep main
        find out the “main” prog PID
    5. gdb -p <PID>
        run gdb by attaching the PID. After gdb runs, the main prog will hange
    6. b main.cpp:35
        set breakpoint at main.cpp, change to the exact line number
    7. c
        continue the main prog. It will break at the breakpoint
    8. print i
        print the value of i
*/

int main(int argc ,char **argv)
{
    std::cout << "Test GDB" << std::endl;

    int i = 1;

    while(i < 2)
    {
        std::cout << "i = " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i++;
    }

    std::string s("abc");
    std::cout << "empty string:" << s.c_str() << std::endl;

    return 0;
}
