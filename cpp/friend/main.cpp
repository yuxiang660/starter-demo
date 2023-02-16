#include <iostream>
#include <string>
#include <typeinfo>

#include "A.h"
#include "B.h"

int main(int argc ,char **argv)
{
    A ax(5);
    PWR::B b(ax);
    b.set(6);
    ax.dump();

    return 0;
}
