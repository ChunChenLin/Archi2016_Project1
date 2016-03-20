#include <stdio.h>
#include <stdlib.h>

FILE *iimage, *dimage;
unsigned iimageLen, dimageLen;
unsigned reg[32], PC;
unsigned op, rs, rt, rd, func, shamt, immediate, address, position;
char *iimageBuffer, *dimageBuffer;
char DMemory[1024], IMemory[1024];
