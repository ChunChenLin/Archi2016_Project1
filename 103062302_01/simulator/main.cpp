#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "load.h"
#include "resolve.h"
#include "translate.h"
#include "instruction.h"
#include "out.h"

FILE *iimage, *dimage, *error_dump, *snapshot;
unsigned iimageLen, dimageLen;
char *iimageBuffer, *dimageBuffer;

int main() {
    Open();
    DImg();
    IImg();

    Terminal::halt = false;
    while(!Terminal::halt) {
    	Terminal::write2Zero = false;
    	Terminal::numberOverflow = false;
    	Terminal::memoryOverflow = false;
    	Terminal::dataMisaaligned = false;
    	snapShot();
    	Assembly();
    	errorDump();
    }
    //errorDump();

    return 0;
}
