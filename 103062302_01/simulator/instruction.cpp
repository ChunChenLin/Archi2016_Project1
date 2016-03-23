#include "instruction.h"
#include "global.h"

/* look up Appendix A */

void detectWrite2Zero(string format) {
	if(format=="R") {
		if(rd==0) {
			write2Zero = true;
			reg[rd] = 0;
		}
	} else { //I format
		if(rt==0) {
			write2Zero = true;
			reg[rt] = 0;
		}
	}
}

// add, sub, addi, lw, lh, lhu, lb,
// lbu, sw, sh, sb, beq, bne
void detectNumberOverflow(string format, bool isPos) {
	unsigned sign_rs, sign_rt, sign_rd, sign_immediate, sign_pos;
	sign_rs = reg[rs]>>31;
	sign_rt = reg[rt]>>31;
	sign_rd = reg[rd]>>31;
	sign_immediate = immediate>>31;
	sign_pos = position>>31;

	if(format=="R") {
		if(sign_rs==sign_rt && sign_rs!=sign_rd) {
			numberOverflow = true;
		}
	} else { //I format
		if(isPos && sign_rs==sign_immediate && sign_rs!=sign_pos) {
			numberOverflow = true;
		} else if(sign_rs==sign_immediate && sign_rs!=sign_rt) {
			numberOverflow = true;
		}
	}
}

bool detectMemoryOverflow(int n) {
	if(position+n >= 1024) {
		memoryOverflow = true;
		halt = true;
		return true;
	} else return false;
}

bool detectDataMisaaligned(int n) {
	if(position%(n+1)>0) {
		dataMisaaligned = true;
		halt = true;
		return true;
	} else return false;
}

void R_format(string op) { /* func is merged into op */
    RsRtRd();

    if(op == "add")	{
    	reg[rd] = reg[rs] + reg[rt];
    	detectWrite2Zero("R");
    	detectNumberOverflow("R", false);
    }
	else if(op == "addu") {
		reg[rd] = reg[rs] + reg[rt];
		detectWrite2Zero("R");
	}
	else if(op == "sub") {
		reg[rd] = reg[rs] - reg[rt];
		detectWrite2Zero("R");
		detectNumberOverflow("R", false);
	}
	else if(op == "and") {
		reg[rd] = reg[rs] & reg[rt];
		detectWrite2Zero("R");
	}
	else if(op == "or")	{
		reg[rd] = reg[rs] | reg[rt];
		detectWrite2Zero("R");
	}
	else if(op == "xor") {
		reg[rd] = reg[rs] ^ reg[rt];
		detectWrite2Zero("R");
	}
	else if(op == "nor") {
		reg[rd] = ~(reg[rs] | reg[rt]);
		detectWrite2Zero("R");
	}
	else if(op == "nand") {
		reg[rd] = ~(reg[rs] & reg[rt]);
		detectWrite2Zero("R");
	}
	else if(op == "slt") {
		reg[rd] = ((int)reg[rs] < (int)reg[rt]) ? 1 : 0;
		detectWrite2Zero("R");
	}
	else if(op == "sll") {
		Shamt(&shamt);
		reg[rd] = reg[rt] << shamt;
		if(rt!=0 || shamt!=0) {
			detectWrite2Zero("R");
		}
	}
	else if(op == "srl") { // remain original pos/neg
		Shamt(&shamt);
		reg[rd] = reg[rt] >> shamt;
		detectWrite2Zero("R");
	}
	else if(op == "sra") { // $d = $t >> C, with sign bit shifted in
		Shamt(&shamt);
		reg[rd] = (int)reg[rt] >> shamt;
		detectWrite2Zero("R");
	}
	else { //if(op == "jr")
		PC = reg[rs];
		return;
	}

	PC += 4; // need to plus 4 immediately
}

void I_format(string op) {
	unsigned t1, t2, t3, t4;

	RsRtRd();

	if(op == "addi") {
		SignedImmediate(&immediate);
		reg[rt] = reg[rs] + immediate;
		detectWrite2Zero("I");
		detectNumberOverflow("I", false);
	}
	else if(op == "addiu") {
		UnsignedImmediate(&immediate);
		reg[rt] = reg[rs] + immediate;
		detectWrite2Zero("I");
	}
	else if(op == "lw") {
		SignedImmediate(&immediate);
		position = reg[rs] + immediate;
		if(detectMemoryOverflow(3)) return;
		if(detectDataMisaaligned(3)) return;
		t1 = DMemory[position] << 24;
		t2 = DMemory[position+1] << 24 >> 8;
		t3 = DMemory[position+2] << 24 >> 16;
		t4 = DMemory[position+3] << 24 >> 24;
		reg[rt] = t1 + t2 + t3 + t4;
		detectWrite2Zero("I");
		detectNumberOverflow("I", true);
	}
	else if(op == "lh") {
		SignedImmediate(&immediate);
		position = reg[rs] + immediate; // need detection
		if(detectMemoryOverflow(1)) return;
		if(detectDataMisaaligned(1)) return;
		t1 = DMemory[position] << 24 >> 16;
		t2 = DMemory[position+1] << 24 >> 24;
		reg[rt] = (short)(t1 + t2);
		detectWrite2Zero("I");
		detectNumberOverflow("I", true);
	}
	else if(op == "lhu") {
		SignedImmediate(&immediate);
		position = reg[rs] + immediate; // need detection
		if(detectMemoryOverflow(1)) return;
		if(detectDataMisaaligned(1)) return;
		t1 = DMemory[position] << 24 >> 16;
		t2 = DMemory[position+1] << 24 >> 24;
		reg[rt] = t1 + t2;
		detectWrite2Zero("I");
		detectNumberOverflow("I", true);
	}
	else if(op == "lb") {
		SignedImmediate(&immediate);
		position = reg[rs] + immediate; // need detection
		if(detectMemoryOverflow(0)) return;
		if(detectDataMisaaligned(0)) return;
		reg[rt] = DMemory[position];
		detectWrite2Zero("I");
		detectNumberOverflow("I", true);
	}
	else if(op == "lbu") {
		SignedImmediate(&immediate);
		position = reg[rs] + immediate; // need detection
		if(detectMemoryOverflow(0)) return;
		if(detectDataMisaaligned(0)) return;
		reg[rt] = DMemory[position] << 24 >> 24;
		detectWrite2Zero("I");
		detectNumberOverflow("I", true);
	}
	else if(op == "sw") {
		SignedImmediate(&immediate);
		position = reg[rs] + immediate; // need detection
		if(detectMemoryOverflow(3)) return;
		if(detectDataMisaaligned(3)) return;
		DMemory[position] = reg[rt] >> 24;
		DMemory[position+1] = reg[rt] << 8 >> 24;
		DMemory[position+2] = reg[rt] << 16 >> 24;
		DMemory[position+3] = reg[rt] << 24 >> 24;
		detectNumberOverflow("I", true);
	}
	else if(op == "sh") {
		SignedImmediate(&immediate);
		position = reg[rs] + immediate; // need detection
		if(detectMemoryOverflow(1)) return;
		if(detectDataMisaaligned(1)) return;
		DMemory[position] = reg[rt] << 16 >> 24;
		DMemory[position+1] = reg[rt] << 24 >> 24;
		detectNumberOverflow("I", true);
	}
	else if(op == "sb") {
		SignedImmediate(&immediate);
		position = reg[rs] + immediate; // need detection
		if(detectMemoryOverflow(0)) return;
		if(detectDataMisaaligned(0)) return;
		DMemory[position] = reg[rt] << 24 >> 24;
		detectNumberOverflow("I", true);
	}
	else if(op == "lui") {
		UnsignedImmediate(&immediate);
		reg[rt] = immediate << 16;
		detectWrite2Zero("I");
	}
	else if(op == "andi") {
		UnsignedImmediate(&immediate);
		reg[rt] = reg[rs] & immediate;
		detectWrite2Zero("I");
	}
	else if(op == "ori") {
		UnsignedImmediate(&immediate);
		reg[rt] = reg[rs] | immediate;
		detectWrite2Zero("I");
	}
	else if(op == "nori") {
		UnsignedImmediate(&immediate);
		reg[rt] = ~(reg[rs] | immediate);
		detectWrite2Zero("I");
	}
	else if(op == "slti") {
		SignedImmediate(&immediate);
		reg[rt] = ((int)reg[rs] < (int)immediate) ? 1 : 0;
		detectWrite2Zero("I");
	}
	else if(op == "beq") {
		SignedImmediate(&immediate);
		if(reg[rs]==reg[rt]) {
			PC += (4 + (immediate << 2));
			return;
		}
	}
	else if(op == "bne") {
		SignedImmediate(&immediate);
		if(reg[rs]!=reg[rt]) {
			PC += (4 + (immediate << 2));
			return;
		}
	}
	else { //if(op == "bgtz")
		SignedImmediate(&immediate);
		if((int)reg[rs] > 0) {
			PC += (4 + (immediate << 2));
			return;
		}
	}

	PC += 4;
}

void J_format(string op) {
	unsigned t1, t2, t3, t4;

	if(op=="jal") reg[31] = PC + 4;

	t1 = IMemory[PC];
	t2 = IMemory[PC+1];
    t3 = IMemory[PC+2];
    t4 = IMemory[PC + 3];
    t1 = t1 << 30 >> 6;
    t2 = t2 << 24 >> 8;
    t3 = t3 << 24 >> 16;
    t4 = t4 << 24 >> 24;
    address = t1 + t2 + t3 + t4;
    PC = ((PC + 4) >> 28 << 28) | (address << 2);
}


