#ifndef instruction_h
#define instruction_h

#include "register.h"
#include<string>

using namespace std;

void R_format(string op, int rs, int rt, int rd, int shamt);
void I_format(string op, int rs, int rt, int imm);
void J_format(string op, unsigned int addr);

#endif 
