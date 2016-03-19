#include "instruction.h"

/* look up Appendix A */

void R_format(string op) { /* func is merged into op */
    if(op == "add")	AllRegister::reg[rd].value = AllRegister::reg[rs].value + AllRegister::reg[rt].value;
	else if(op == "addu") AllRegister::reg[rd].value = AllRegister::reg[rs].value + AllRegister::reg[rt].value; /* unsigned */
	else if(op == "sub") AllRegister::reg[rd].value = AllRegister::reg[rs].value - AllRegister::reg[rt].value;
	else if(op == "and") AllRegister::reg[rd].value = AllRegister::reg[rs].value & AllRegister::reg[rt].value;
	else if(op == "or")	AllRegister::reg[rd].value = AllRegister::reg[rs].value | AllRegister::reg[rt].value;
	else if(op == "xor") AllRegister::reg[rd].value = AllRegister::reg[rs].value ^ AllRegister::reg[rt].value;
	else if(op == "nor") AllRegister::reg[rd].value = ~(AllRegister::reg[rs].value | AllRegister::reg[rt].value);
	else if(op == "nand") AllRegister::reg[rd].value = ~(AllRegister::reg[rs].value & AllRegister::reg[rt].value);
	else if(op == "slt") AllRegister::reg[rd].value = (AllRegister::reg[rs].value < AllRegister::reg[rt].value);

	else if(op == "sll") AllRegister::reg[rd].value = AllRegister::reg[rt].value << shamt;
	else if(op == "srl") { /* remain original pos/neg */
		int temp = AllRegister::reg[rt].value >> shamt;
		bitset<32> bs = temp;
		/* 
  			......................
		*/
	}
	else if(op == "sra") AllRegister::reg[rd].value = AllRegister::reg[rt].value >> shamt;
	else if(op == "jr") AllRegister::PC.value = AllRegister::reg[rs].value; return; /* don't need to plus 4*/

	AllRegister::PC.value += 4; /* need to plus 4 immediately*/ 
}

void I_format(string op) {
	if(op == "addi") AllRegister::reg[rt].value = AllRegister.reg[rs].value + imm;
	else if(op == "addiu") {
		unsigned int ui = imm; 
		AllRegister::reg[rt].value = AllRegister::reg[rs].value + ui;
	}
	else if(op == "lw") {
		/* $t = 4 bytes from Memory[$s + C(signed)] */
		int Word[32];
		memset(Word, 0, sizeof(Word));
		for(int i=4; i>0; i--) { // byte
			for(int j=0; j<8; j++) { // bit
				Word[i*8-1-j] = DataMemory::Memory[AllRegister::reg[rs].value+imm+(4-i)][7-j];
			}
		}
		AllRegister::reg[rt].value = /* signed */;
	}
	else if(op == "lh") {
		/* $t = 2 bytes from Memory[$s + C(signed)], signed */
		int Word[32];
		memset(Word, 0, sizeof(Word));
		for(int i=2; i>0; i--) {
			for(int j=0; j<8; j++) {
				Word[i*8-1-j] = DataMemory::Memory[AllRegister::reg[rs].value+imm+(2-i)][7-j];
			}
		}
		AllRegister::reg[rt].value = /* signed */;
	}
	else if(op == "lhu") {
		/* $t = 2 bytes from Memory[$s + C(signed)], unsigned */
		int Word[32];
		memset(Word, 0, sizeof(Word));
		for(int i=2; i>0; i--) {
			for(int j=0; j<8; j++) {
				Word[i*8-1-j] = DataMemory::Memory[AllRegister::reg[rs].value+imm+(2-i)][7-j];
			}
		}
		AllRegister::reg[rt].value = /* unsigned */;
	}
	else if(op == "lb") {
		/* $t = Memory[$s + C(signed)], signed */
		int Word[32];
		memset(Word, 0, sizeof(Word));
		for(int i=0; i<8; i++) {
			Word[i] = DataMemory::Memory[AllRegister::reg[rs].value+imm][i];
		}
		AllRegister::reg[rt].value = /* signed */;
	}
	else if(op == "lbu") {
		/* $t = Memory[$s + C(signed)], unsigned */
		int Word[32];
		memset(Word, 0, sizeof(Word));
		for(int i=0; i<8; i++) {
			Word[i] = DataMemory::Memory[AllRegister::reg[rs].value+imm][i];
		}
		AllRegister::reg[rt].value = /* unsigned */;
	}
	else if(op == "sw") {
		/* 4 bytes from Memory[$s + C(signed)] = $t */
		bitset<32> bs;
		bs = AllRegister::reg[rt].value;
		for(int i=4; i>0; i--) { // byte
			for(int j=0; j<8; j++) { //bit
				DataMemory::Memory[AllRegister::reg[rs].value+imm+(4-i)].push_back(bs[(i-1)*8+j]);
			}
		}
	}
	else if(op == "sh") {
		/* 2 bytes from Memory[$s + C(signed)] = $t & 0x0000FFFF */
		bitset<32> bs;
		bs = AllRegister::reg[rt].value;
		for(int i=2; i>0; i--) {
			for(int j=0; j<8; j++) {
				DataMemory::Memory[AllRegister::reg[rs].value+imm+(2-i)].push_back(bs[(i-1)*8+j]);
			}
		}
	}
	else if(op == "sb") {
		/* Memory[$s + C(signed)] = $t & 0x000000FF */
		bitset<32> bs;
		bs = AllRegister::reg[rt].value;
		for(int i=0; i<8; i++) {
			DataMemory::Memory[AllRegister::reg[rs].value+imm].push_back(bs[j]);
		}
	}
	else if(op == "andi") {
		unsigned int ui = imm;
		AllRegister::reg[rt].value = AllRegister::reg[rs].value & ui;
	}
	else if(op == "ori") {
		unsigned int ui = imm;
		AllRegister::reg[rt].value = AllRegister::reg[rs].value & ui;
	}
	else if(op == "nori") {
		unsigned int  ui = imm;
		AllRegister::reg[rt].value = AllRegister::reg[rs].value ^ ui;
	}
	else if(op == "slti") {
		AllRegister::reg[rt].value = (AllRegister::reg[rs] < imm);
	}
	else if(op == "beq") {
		/* if ($s == $t) go to PC+4+4*C(signed) */ 
		if(AllRegister::reg[rs].value == AllRegister::reg[rt].value) {
			AllRegister::PC.value = AllRegister::PC.value + 4 + 4 * imm; 
			return;
		}
	}
	else if(op == "bne") {
		/* if ($s != $t) go to PC+4+4*C(signed) */
		if(AllRegister::reg[rs].value != AllRegister::reg[rt].value) {
			AllRegister::PC.value = AllRegister::PC.value + 4 + 4 * imm; 
			return;
		}
	}
	else if(op == "lui") {
		AllRegister::reg[rt].value = imm << 16;
		return;
	}
	else if(op == "bgtz") {
		/* if ($s > 0) go to PC+4+4*C(signed) */
		if(AllRegister::reg[rs].value > 0) {
			AllRegister::PC.value = AllRegister::PC.value + 4 + 4 * imm;
			return;
		}
	}

	AllRegister::PC.value += 4;
}

void J_format(string op) {
	if(op == "jal") AllRegister::reg[31].value = AllRegister::PC.value + 4;  /* $31 = PC + 4; */

	/* PC = (PC+4)[31:28] | 4*C(unsigned) */
	AllRegister::PC.value += 4; /* [31:28] + [27:0] */
	addr = addr << 2; /* 4*C */

	int Word[32];
	memset(Word, 0, sizeof(Word));
	bitset<32> b1, b2;
	b1 = AllRegister::PC.value;
	b2 = addr;
	
	for(int i=0; i<=27; i++) Word[i] = b2[i];
	for(int i=28; i<=31; i++) Word[i] = b1[i];

	AllRegister::PC.value = /* ......... */;
}  


