#include "instruction.h"
#include "global.h"

/* look up Appendix A */

inline void detectWrite2Zero(string format) {
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
inline void detectNumberOverflow(string format, bool sub, bool isPos) {
	unsigned sign_rs, sign_rt, sign_rd, sign_immediate, sign_pos;
	sign_rs = reg[rs]>>31;
	//The subtraction a - b is done as addition a + (-b).
	sign_rt = (sub==true)?(-reg[rt])>>31:reg[rt]>>31;
	sign_rd = reg[rd]>>31;
	sign_immediate = immediate>>31;
	sign_pos = position>>31;

	if(format=="R") {
		if(sign_rs==sign_rt && sign_rs!=sign_rd) {
			numberOverflow = true;
		}
	} else { //I format
		if(isPos) {
			if(sign_rs==sign_immediate && sign_rs!=sign_pos) {
				numberOverflow = true;
			}
		} else if(sign_rs==sign_immediate && sign_rs!=sign_rt) {
			numberOverflow = true;
		}
	}
}

inline bool detectMemoryOverflow(int n) {
	if(position+n >= 1024) {
		memoryOverflow = true;
		halt = true;
		return true;
	} else return false;
}

inline bool detectDataMisaaligned(int n) {
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
    	detectNumberOverflow("R", false, false);
    }
	else if(op == "addu") {
		reg[rd] = reg[rs] + reg[rt];
		detectWrite2Zero("R");
	}
	else if(op == "sub") {
		reg[rd] = reg[rs] - reg[rt];
		detectWrite2Zero("R");
		detectNumberOverflow("R", true, false);
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
		Shamt();
		reg[rd] = reg[rt] << shamt;
		if(rt!=0 || shamt!=0) {
			detectWrite2Zero("R");
		}
	}
	else if(op == "srl") { // remain original pos/neg
		Shamt();
		reg[rd] = reg[rt] >> shamt;
		detectWrite2Zero("R");
	}
	else if(op == "sra") { // $d = $t >> C, with sign bit shifted in
		Shamt();
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
	int inT1, inT2, intT3, inT4;
	bool isMemoryOverflow = false, isDataMisaaligned = false;

	RsRtRd();

	if(op == "addi") {
		SignedImmediate();
		reg[rt] = reg[rs] + immediate;
		detectWrite2Zero("I");
		detectNumberOverflow("I", false, false);
	}
	else if(op == "addiu") {
		//UnsignedImmediate();
	    SignedImmediate();
		reg[rt] = reg[rs] + immediate;
		detectWrite2Zero("I");
	}
	else if(op == "lw") {
		SignedImmediate();
		position = reg[rs] + immediate;
		/*if(detectMemoryOverflow(3)) return;
		if(detectDataMisaaligned(3)) return;*/
		isMemoryOverflow = detectMemoryOverflow(3);
		isDataMisaaligned = detectDataMisaaligned(3);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			t1 = DMemory[position] << 24;
			t2 = DMemory[position+1] << 16;
			t3 = DMemory[position+2] << 8;
			t4 = DMemory[position+3];
			reg[rt] = t1 + t2 + t3 + t4;
		} 
		//if(cycle==212) printf("%X\n",position);
		/*printf("%d %d %d %d %x\n",
			DMemory[position],
			DMemory[position+1],
			DMemory[position+2],
			DMemory[position+3],
			reg[rt]
			);*/
		detectWrite2Zero("I");
		detectNumberOverflow("I", false, true);
	}
	else if(op == "lh") {
		SignedImmediate();
		position = reg[rs] + immediate; // need detection
		//if(detectMemoryOverflow(1)) return;
		//if(detectDataMisaaligned(1)) return;
		isMemoryOverflow = detectMemoryOverflow(1);
		isDataMisaaligned = detectDataMisaaligned(1);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			t1 = DMemory[position];//<< 8;//<< 24 >> 16;
			t1 = t1 << 24 >> 16;
			t2 = DMemory[position+1];//<< 24 >> 24;
			t2 = t2 << 24 >> 24;
			reg[rt] = (short)(t1 + t2);
		} 
		/*inT1 = DMemory[position];
		inT1 = inT1 << 24 >> 16;
		inT2 = DMemory[position+1];
		inT2 = inT2 << 24 >> 24;
		reg[rt] = (short)(inT1+inT2);*/
		detectWrite2Zero("I");
		detectNumberOverflow("I", false, true);
	}
	else if(op == "lhu") {
		SignedImmediate();
		position = reg[rs] + immediate; // need detection
		//if(detectMemoryOverflow(1)) return;
		//if(detectDataMisaaligned(1)) return;
		isMemoryOverflow = detectMemoryOverflow(1);
		isDataMisaaligned = detectDataMisaaligned(1);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			t1 = DMemory[position] << 8;//<< 24 >> 16;
			t2 = DMemory[position+1]; //<< 24 >> 24;
			reg[rt] = t1 + t2;
		} 
		detectWrite2Zero("I");
		detectNumberOverflow("I", false, true);
	}
	else if(op == "lb") {
		SignedImmediate();
		position = reg[rs] + immediate; // need detection
		//if(detectMemoryOverflow(0)) return;
		//if(detectDataMisaaligned(0)) return;
		isMemoryOverflow = detectMemoryOverflow(0);
		isDataMisaaligned = detectDataMisaaligned(0);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			//reg[rt] = DMemory[position]<<24>>24;//signExtension(DMemory[position]);//DMemory[position];
			inT1 = DMemory[position];
			inT1 = inT1 << 24 >> 24;
			reg[rt] = inT1;
		} 
		detectWrite2Zero("I");
		detectNumberOverflow("I", false, true);
	}
	else if(op == "lbu") {
		SignedImmediate();
		position = reg[rs] + immediate; // need detection
		//if(detectMemoryOverflow(0)) return;
		//if(detectDataMisaaligned(0)) return;
		isMemoryOverflow = detectMemoryOverflow(0);
		isDataMisaaligned = detectDataMisaaligned(0);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			reg[rt] = DMemory[position];//<< 24 >> 24;
		} 
		detectWrite2Zero("I");
		detectNumberOverflow("I", false, true);
	}
	else if(op == "sw") {
		SignedImmediate();
		position = reg[rs] + immediate; // need detection
		//if(detectMemoryOverflow(3)) return;
		//if(detectDataMisaaligned(3)) return;
		isMemoryOverflow = detectMemoryOverflow(3);
		isDataMisaaligned = detectDataMisaaligned(3);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			//DMemory[position] = reg[rt] >> 24;
			//DMemory[position+1] = reg[rt] << 8 >> 24;
			//DMemory[position+2] = reg[rt] << 16 >> 24;
			//DMemory[position+3] = reg[rt] << 24 >> 24;
			DMemory[position] = reg[rt] >> 24;
			DMemory[position + 1] = (reg[rt] >> 16) & 0xff;
			DMemory[position + 2] = (reg[rt] >> 8) & 0xff;
			DMemory[position + 3] = (reg[rt]) & 0xff;
		} 
		detectNumberOverflow("I", false, true);
	}
	else if(op == "sh") {
		SignedImmediate();
		position = reg[rs] + immediate; // need detection
		//if(detectMemoryOverflow(1)) return;
		//if(detectDataMisaaligned(1)) return;
		isMemoryOverflow = detectMemoryOverflow(1);
		isDataMisaaligned = detectDataMisaaligned(1);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			DMemory[position] = (reg[rt] >> 8) & 0xff;//reg[rt] << 16 >> 24;
			DMemory[position+1] = reg[rt]; //<< 24 >> 24;
		} 
		detectNumberOverflow("I", false, true);
	}
	else if(op == "sb") {
		SignedImmediate();
		position = reg[rs] + immediate; // need detection
		//if(detectMemoryOverflow(0)) return;
		//if(detectDataMisaaligned(0)) return;
		isMemoryOverflow = detectMemoryOverflow(0);
		isDataMisaaligned = detectDataMisaaligned(0);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			DMemory[position] = reg[rt]; //<< 24 >> 24;
		} 
		detectNumberOverflow("I", false, true);
	}
	else if(op == "lui") {
		UnsignedImmediate();
		reg[rt] = immediate << 16;
		detectWrite2Zero("I");
	}
	else if(op == "andi") {
		UnsignedImmediate();
		reg[rt] = reg[rs] & immediate;
		detectWrite2Zero("I");
	}
	else if(op == "ori") {
		UnsignedImmediate();
		reg[rt] = reg[rs] | immediate;
		detectWrite2Zero("I");
	}
	else if(op == "nori") {
		UnsignedImmediate();
		reg[rt] = ~(reg[rs] | immediate);
		detectWrite2Zero("I");
	}
	else if(op == "slti") {
		SignedImmediate();
		reg[rt] = ((int)reg[rs] < (int)immediate) ? 1 : 0;
		detectWrite2Zero("I");
	}
	else if(op == "beq") {
		SignedImmediate();
		if(reg[rs]==reg[rt]) {
			PC += (4 + (immediate << 2));
			return;
		}
	}
	else if(op == "bne") {
		SignedImmediate();
		if(reg[rs]!=reg[rt]) {
			PC += (4 + (immediate << 2));
			return;
		}
	}
	else { //if(op == "bgtz")
		SignedImmediate();
		if((int)reg[rs] > 0) {
			PC += (4 + (immediate << 2));
			return;
		}
	}

	PC += 4;
}

void J_format(string op) {
	if(op=="jal") reg[31] = PC + 4;
    Address();
    PC = ((PC + 4) >> 28 << 28) | (address << 2);
}


