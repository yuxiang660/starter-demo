#include "register.h"
#include <iostream>

void foo()
{
   std::cout << "foo:" << std::hex << SD_DCC_REG_ZERO__rsvd1_LSB << std::endl;
}
