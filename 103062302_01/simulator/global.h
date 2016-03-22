#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

extern FILE *iimage, *dimage, *error_dump, *snapshot;
extern bool halt;
extern bool write2Zero, numberOverflow, memoryOverflow, dataMisaaligned;
extern unsigned iimageLen, dimageLen;
extern unsigned reg[32], PC, cycle;
extern unsigned op, rs, rt, rd, func, shamt, immediate, address, position;
extern char *iimageBuffer, *dimageBuffer;
extern char DMemory[1024], IMemory[1024];


