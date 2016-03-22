#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "load.h"
#include "resolve.h"
#include "translate.h"
#include "instruction.h"

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
