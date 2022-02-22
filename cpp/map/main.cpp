#include <map>
#include <iostream>

int main()
{
  std::map<int, int> m;
  m[0] = 1;
  m[2] = 3;

  for (auto iter : m)
  {
    std::cout << "key: " << iter.first << ", val: " << iter.second << std::endl;
  }

  return 0;
}
