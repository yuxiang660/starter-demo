#include <iostream>
#include <string>
#include <typeinfo>

class Bar
{
public:
    Bar() = default;
    ~Bar() { std::cout << "destructor\n"; }
    void foo()
    {
        if (done)
            return;
        std::cout << "Bar::foo()" << std::endl;
        done = true;
    }
private:
    bool done = false;
};

int main(int argc ,char **argv)
{
    {
        Bar b;
        b.foo();
        b.foo();
    }
    {
        Bar b;
        b.foo();
        b.foo();
    }
    return 0;
}
