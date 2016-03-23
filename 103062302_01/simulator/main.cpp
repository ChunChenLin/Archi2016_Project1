#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "load.h"
#include "resolve.h"
#include "translate.h"
#include "instruction.h"
#include "out.h"

FILE *iimage, *dimage, *error_dump, *snapshot;
bool halt;
bool write2Zero, numberOverflow, memoryOverflow, dataMisaaligned;
unsigned iimageLen, dimageLen;
unsigned reg[32], PC, cycle;
unsigned op, rs, rt, rd, func, shamt, immediate, address, position;
char *iimageBuffer, *dimageBuffer;
unsigned char DMemory[1024], IMemory[1024];

int main() {
    Open();
    DImg();
    IImg();

    halt = false;
    while(!halt) {
    	write2Zero = false;
    	numberOverflow = false;
    	memoryOverflow = false;
    	dataMisaaligned = false;
    	snapShot();
    	Assembly();
    	errorDump();
    }
    //errorDump();

    return 0;
}
