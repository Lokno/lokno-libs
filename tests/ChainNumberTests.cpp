#include <iostream>
#include <cassert>
#include "ChainNumber.h"

int main()
{
    ChainNumber base,power,one,three,four;

    base++,base++;
    power++,power++,power++,power++,power++;
    four++,four++,four++,four++;
    one++;
    three++;three++;three++;

    assert( ((((--ChainNumber::pow(base,power) + one) / four - four) * four) % three).toInt() == 1 );

    return 0;
}