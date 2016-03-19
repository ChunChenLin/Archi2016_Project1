#include <stdio.h>
#include <stdlib.h>

FILE *iimage, *dimage;
unsigned iimageLen, dimageLen;
unsigned reg[32], PC;
char *iimageBuffer, *dimageBuffer;
char DMemory[1024], IMemory[1024];
