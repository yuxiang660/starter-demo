#pragma once

#include <iostream>
#include <string>
#include <typeinfo>

namespace PWR { class B; }

class A
{
   friend class PWR::B;

public:
   A(int a) : m_a(a) {}
   void dump() { std::cout << "m_a:" << m_a << std::endl; }

private:
   int m_a = 0;
};