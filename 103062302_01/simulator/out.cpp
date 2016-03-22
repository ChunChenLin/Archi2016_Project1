#include "out.h"

void errorDump() {
	if (write2Zero) {
        fprintf(error_dump, "In cycle %d: Write $0 Error\n", cycle);
    }
    if (numberOverflow) {
        fprintf(error_dump, "In cycle %d: Number Overflow\n", cycle);
    }
    if (memoryOverflow) {
        fprintf(error_dump, "In cycle %d: Address Overflow\n", cycle);
    }
    if (dataMisaaligned) {
        fprintf(error_dump, "In cycle %d: Misalignment Error\n", cycle);
    }
}

void snapShot() {
	fprintf(snapshot, "cycle %d\n", cycle);
	for(int i=0; i<32; i++) {
		fprintf(snapshot, "$%2d: 0x", i);
		fprintf(snap, "%8x\n", reg[i]);
	}
	fprintf(snapshot, "PC: 0x%8x\n\n\n", PC);
	cycle++;
}