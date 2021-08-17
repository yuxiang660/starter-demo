#include "register.h"
#include <iostream>

void bar()
{
   std::cout << "bar:" << std::hex << SD_DCC_REG_ZERO__rsvd1_MASK << std::endl;
}
