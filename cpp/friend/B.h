#pragma once

#include "A.h"

#include <iostream>
#include <string>
#include <typeinfo>

namespace PWR
{

   class B
   {
   public:
      B(A &a) : m_a(a) {}
      void set(int a) { m_a.m_a = a; }

   private:
      A &m_a;
   };

}
