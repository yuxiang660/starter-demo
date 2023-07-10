#include <iostream>
#include <string>
#include <typeinfo>
#include <memory>
#include <assert.h>

class Bar
{
public:
    Bar() = default;
    void bar()
    {
        std::cout << "Bar::bar\n";
    }
};

class Foo
{
public:
    Foo() = default;
    void foo()
    {
        assert(m_bar.get() != nullptr);
        m_bar->bar();
    }
private:
    std::unique_ptr<Bar> m_bar = std::make_unique<Bar>();
};

int main(int argc ,char **argv)
{
    Foo f;
    f.foo();
    return 0;
}
