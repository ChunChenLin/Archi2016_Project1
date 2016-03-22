#include "out.h"

void errorDump() {
	if (write2Zero) {
        //fprintf(error_dump, "In cycle %d: Write $0 Error\n", cycle);
        printf("In cycle %d: Write $0 Error\n", cycle);
    }
    if (numberOverflow) {
        //fprintf(error_dump, "In cycle %d: Number Overflow\n", cycle);
        printf("In cycle %d: Number Overflow\n", cycle);
    }
    if (memoryOverflow) {
        //fprintf(error_dump, "In cycle %d: Address Overflow\n", cycle);
        printf("In cycle %d: Address Overflow\n", cycle);
    }
    if (dataMisaaligned) {
        //fprintf(error_dump, "In cycle %d: Misalignment Error\n", cycle);
        printf("In cycle %d: Misalignment Error\n", cycle);
    }
}

void snapShot() {
	//fprintf(snapshot, "cycle %d\n", cycle);
	printf("cycle %d\n", cycle);
	for(int i=0; i<32; i++) {
		//fprintf(snapshot, "$%2d: 0x", i);
		//fprintf(snapshot, "%8x\n", reg[i]);
		printf("$%2d: 0x", i);
		printf("%8x\n", reg[i]);
	}
	//fprintf(snapshot, "PC: 0x%8x\n\n\n", PC);
	printf("PC: 0x%8x\n\n\n", PC);
	cycle++;
}
