#include "out.h"

void errorDump() {
	if (Terminal::Terminal::write2Zero) {
        //fprintf(error_dump, "cycle %d: Write $0 Error\n", Register::cycle);
        printf("cycle %d: Write $0 Error\n", Register::cycle);
    }
    if (Terminal::numberOverflow) {
        //fprintf(error_dump, "cycle %d: Number Overflow\n", Register::cycle);
        printf("cycle %d: Number Overflow\n", Register::cycle);
    }
    if (Terminal::memoryOverflow) {
        //fprintf(error_dump, "cycle %d: Address Overflow\n", Register::cycle);
        printf("cycle %d: Address Overflow\n", Register::cycle);
    }
    if (Terminal::dataMisaaligned) {
        //fprintf(error_dump, "cycle %d: Misalignment Error\n", Register::cycle);
        printf("cycle %d: Misalignment Error\n", Register::cycle);
    }
}

void snapShot() {
	//fprintf(snapshot, "cycle %d\n", Register::cycle);
	printf("Register::cycle %d\n", Register::cycle);
	for(int i=0; i<32; i++) {
		//fprintf(snapshot, "$%02d: 0x", i);
		//fprintf(snapshot, "%08X\n", CPURegister::reg[i].value);
		printf("$%02d: 0x", i);
		printf("%08X\n", Register::reg[i]);
	}
	//fprintf(snapshot, "PC: 0x%08X\n\n\n", CPURegister::PC.value);
	printf("PC: 0x%08X\n\n\n", Register::PC);
	Register::cycle++;
}
