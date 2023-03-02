#include <iostream>
#include <string>
#include <typeinfo>

class Base
{
public:
    Base() = default;
    virtual ~Base() = default;
    virtual void foo()
    {
        std::cout << "Base foo" << std::endl;
    }
};

class Derive : public Base
{
public:
    Derive() = default;
    void foo() override
    {
        std::cout << "Derive foo" << std::endl;
    }
};

class Bar : public Derive
{
public:
    Bar() = default;
    void foo() override
    {
        Base::foo();
    }
};

int main(int argc ,char **argv)
{
    Bar b;
    b.foo();
    return 0;
}
