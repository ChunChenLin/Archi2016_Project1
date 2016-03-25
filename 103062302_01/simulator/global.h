#ifndef global_h
#define global_h

#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

class Register {
public:
	static unsigned reg[32], PC, cycle;
};

class Memory {
public:
	static unsigned char DMemory[1024], IMemory[1024];
	static unsigned address, position;
};

class Instruction {
public:
	static unsigned op, rs, rt, rd, func, shamt, immediate;
};

class Terminal {
public:
	static bool halt;
	static bool write2Zero, numberOverflow, memoryOverflow, dataMisaaligned;
};

#endif
