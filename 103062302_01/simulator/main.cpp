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

    while(1) {
    	Assembly();
    }

    return 0;
}
