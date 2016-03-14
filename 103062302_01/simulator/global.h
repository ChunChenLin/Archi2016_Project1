#ifndef global_h
#define global_h

#include "register.h"

using namespace std;

class AllRegister {
public:
    Register reg[32], PC; /* both have .value & .Hex() */
}
