#include <iostream>
#include <vector>

class Base
{
public:
  void fooWrapper()
  {
    std::cout << "Base::fooWrapper()" << std::endl;
    foo();
  }

  virtual inline void foo()
  {
    std::cout << "Base::foo()" << std::endl;
  }
};

class Father : public Base
{
public:
  void foo()
  {
    std::cout << "Father::foo()" << std::endl;
  }

  void bar(const std::vector<int>& data)
  {
    for (auto d : data) {
      std::cout << d << " ";
    }
    std::cout << std::endl;
  }
};

int main()
{
  std::cout << "hello" << std::endl;
  Father f;
  f.fooWrapper();
  f.bar({1, 4, 8, 10});
  return 0;
}
