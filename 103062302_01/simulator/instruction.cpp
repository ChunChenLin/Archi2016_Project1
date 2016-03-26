#include "instruction.h"
#include "global.h"
#include <bitset>

/* look up Appendix A */
#include <iostream>
using namespace std;

inline void detectWrite2Zero(string format) {
	if(format=="R") {
		if(Instruction::rd==0) {
			Terminal::write2Zero = true;
			Register::reg[Instruction::rd] = 0;
		}
	} else { //I format
		if(Instruction::rt==0) {
			Terminal::write2Zero = true;
			Register::reg[Instruction::rt] = 0;
		}
	}
}

// add, sub, addi, lw, lh, lhu, lb,
// lbu, sw, sh, sb, beq, bne
// a + b = c;
inline void detectNumberOverflow(int a, int b, int c) { 
	if((a>0&&b>0&&c<0) || (a<0&&b<0&&c>0)) {
		Terminal::numberOverflow = true;
	} 
}

inline bool detectMemoryOverflow(int n) {
	if(Memory::position + n >= 1024 || Memory::position >= 1024) {
		Terminal::memoryOverflow = true;
		Terminal::halt = true;
		return true;
	} else return false;
}

inline bool detectDataMisaaligned(int n) {
	if(Memory::position % (n+1) > 0) {
		Terminal::dataMisaaligned = true;
		Terminal::halt = true;
		return true;
	} else return false;
}

void R_format(string op) { /* func is merged into op */
    RsRtRd();
    signed int Irs,Irt,Ird;
    unsigned signRs, signRt, signRd;
    //if(Register::cycle==20||Register::cycle==22) std::cout<<op<<std::endl;

    if(op == "add")	{
    	signRs = Register::reg[Instruction::rs] >> 31, signRt = Register::reg[Instruction::rt] >> 31;
        Register::reg[Instruction::rd] = Register::reg[Instruction::rs] + Register::reg[Instruction::rt];
        signRd = Register::reg[Instruction::rd] >> 31;
        if (Instruction::rd == 0) {
            Terminal::write2Zero = true;
            Register::reg[Instruction::rd] = 0;
        }
        if (signRs == signRt && signRs != signRd) {
            Terminal::numberOverflow = true;
        }
    }
	else if(op == "addu") {
		Register::reg[Instruction::rd] = Register::reg[Instruction::rs] + Register::reg[Instruction::rt];
		detectWrite2Zero("R");
	}
	else if(op == "sub") {
		signRs = Register::reg[Instruction::rs] >> 31, signRt = (-Register::reg[Instruction::rt]) >> 31;
        Register::reg[Instruction::rd] = Register::reg[Instruction::rs] - Register::reg[Instruction::rt];
        signRd = Register::reg[Instruction::rd] >> 31;
        if (Instruction::rd == 0) {
            Terminal::write2Zero = true;
            Register::reg[Instruction::rd] = 0;
        }
        if (signRs == signRt && signRs != signRd) {
            Terminal::numberOverflow = true;
        }
	}
	else if(op == "and") {
		Register::reg[Instruction::rd] = Register::reg[Instruction::rs] & Register::reg[Instruction::rt];
		detectWrite2Zero("R");
	}
	else if(op == "or")	{
		Register::reg[Instruction::rd] = Register::reg[Instruction::rs] | Register::reg[Instruction::rt];
		detectWrite2Zero("R");
	}
	else if(op == "xor") {
		Register::reg[Instruction::rd] = Register::reg[Instruction::rs] ^ Register::reg[Instruction::rt];
		detectWrite2Zero("R");
	}
	else if(op == "nor") {
		Register::reg[Instruction::rd] = ~(Register::reg[Instruction::rs] | Register::reg[Instruction::rt]);
		detectWrite2Zero("R");
	}
	else if(op == "nand") {
		Register::reg[Instruction::rd] = ~(Register::reg[Instruction::rs] & Register::reg[Instruction::rt]);
		detectWrite2Zero("R");
	}
	else if(op == "slt") {
		Register::reg[Instruction::rd] = ((int)Register::reg[Instruction::rs] < (int)Register::reg[Instruction::rt]) ? 1 : 0;
		detectWrite2Zero("R");
	}
	else if(op == "sll") {
		Shamt();
		Register::reg[Instruction::rd] = Register::reg[Instruction::rt] << Instruction::shamt;
		if(Instruction::rt!=0 || Instruction::shamt!=0) {
			detectWrite2Zero("R");
		}
	}
	else if(op == "srl") { // remain original pos/neg
		Shamt();
		Register::reg[Instruction::rd] = Register::reg[Instruction::rt] >> Instruction::shamt;
		detectWrite2Zero("R");
	}
	else if(op == "sra") { // $d = $t >> C, with sign bit shifted in
		Shamt();
		Register::reg[Instruction::rd] = (int)Register::reg[Instruction::rt] >> Instruction::shamt;
		detectWrite2Zero("R");
	}
	else { //if(op == "jr")
		Register::PC = Register::reg[Instruction::rs];
		return;
	}

	Register::PC += 4; // need to plus 4 immediately
}

void I_format(string op) {
	unsigned t1, t2, t3, t4;
	int inT1, inT2, intT3, inT4;
	bool isMemoryOverflow = false, isDataMisaaligned = false;

	RsRtRd();
	signed int Irs,Irt,Ird,Iimm,Ipos;
    Irs = (int)Register::reg[Instruction::rs];
    Irt = (int)Register::reg[Instruction::rt];

    //if(Register::cycle==20||Register::cycle==22) std::cout<<op<<std::endl;

	if(op == "addi") {
		SignedImmediate();
		Register::reg[Instruction::rt] = Register::reg[Instruction::rs] + Instruction::immediate;
		detectWrite2Zero("I");
		Iimm = (int)Instruction::immediate;
		Irt = Irs + Iimm;
    	detectNumberOverflow(Irs,Iimm,Irt);
		//detectNumberOverflow("I", false, false);
	}
	else if(op == "addiu") {
		//UnsignedImmediate();
	    SignedImmediate();
		Register::reg[Instruction::rt] = Register::reg[Instruction::rs] + Instruction::immediate;
		detectWrite2Zero("I");
	}
	else if(op == "lw") {
		SignedImmediate();
		Memory::position = Register::reg[Instruction::rs] + Instruction::immediate;
		/*if(detectMemoryOverflow(3)) return;
		if(detectDataMisaaligned(3)) return;*/
		isMemoryOverflow = detectMemoryOverflow(3);
		isDataMisaaligned = detectDataMisaaligned(3);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			t1 = Memory::DMemory[Memory::position] << 24;
			t2 = Memory::DMemory[Memory::position+1] << 16;
			t3 = Memory::DMemory[Memory::position+2] << 8;
			t4 = Memory::DMemory[Memory::position+3];
			Register::reg[Instruction::rt] = t1 + t2 + t3 + t4;
		}
		//if(cycle==212) printf("%X\n",Memory::position);
		/*printf("%d %d %d %d %x\n",
			Memory::DMemory[Memory::position],
			Memory::DMemory[Memory::position+1],
			Memory::DMemory[Memory::position+2],
			Memory::DMemory[Memory::position+3],
			Register::reg[Instruction::rt]
			);*/
		detectWrite2Zero("I");
		Iimm = (int)Instruction::immediate;
		Ipos = Irs + Iimm;
    	detectNumberOverflow(Irs,Iimm,Ipos);
		//detectNumberOverflow("I", false, true);
	}
	else if(op == "lh") {
		SignedImmediate();
		Memory::position = Register::reg[Instruction::rs] + Instruction::immediate; // need detection
		//if(detectMemoryOverflow(1)) return;
		//if(detectDataMisaaligned(1)) return;
		isMemoryOverflow = detectMemoryOverflow(1);
		isDataMisaaligned = detectDataMisaaligned(1);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			t1 = Memory::DMemory[Memory::position];//<< 8;//<< 24 >> 16;
			t1 = t1 << 24 >> 16;
			t2 = Memory::DMemory[Memory::position+1];//<< 24 >> 24;
			t2 = t2 << 24 >> 24;
			Register::reg[Instruction::rt] = (short)(t1 + t2);
		}
		/*inT1 = Memory::DMemory[Memory::position];
		inT1 = inT1 << 24 >> 16;
		inT2 = Memory::DMemory[Memory::position+1];
		inT2 = inT2 << 24 >> 24;
		Register::reg[Instruction::rt] = (short)(inT1+inT2);*/
		detectWrite2Zero("I");
		Iimm = (int)Instruction::immediate;
		Ipos = Irs + Iimm;
    	detectNumberOverflow(Irs,Iimm,Ipos);
		//detectNumberOverflow("I", false, true);
	}
	else if(op == "lhu") {
		SignedImmediate();
		Memory::position = Register::reg[Instruction::rs] + Instruction::immediate; // need detection
		//if(detectMemoryOverflow(1)) return;
		//if(detectDataMisaaligned(1)) return;
		isMemoryOverflow = detectMemoryOverflow(1);
		isDataMisaaligned = detectDataMisaaligned(1);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			t1 = Memory::DMemory[Memory::position] << 8;//<< 24 >> 16;
			t2 = Memory::DMemory[Memory::position+1]; //<< 24 >> 24;
			Register::reg[Instruction::rt] = t1 + t2;
		}
		detectWrite2Zero("I");
		Iimm = (int)Instruction::immediate;
		Ipos = Irs + Iimm;
    	detectNumberOverflow(Irs,Iimm,Ipos);
		//detectNumberOverflow("I", false, true);
	}
	else if(op == "lb") {
		SignedImmediate();
		Memory::position = Register::reg[Instruction::rs] + Instruction::immediate; // need detection
		//if(detectMemoryOverflow(0)) return;
		//if(detectDataMisaaligned(0)) return;
		isMemoryOverflow = detectMemoryOverflow(0);
		isDataMisaaligned = detectDataMisaaligned(0);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			//Register::reg[Instruction::rt] = Memory::DMemory[Memory::position]<<24>>24;//signExtension(Memory::DMemory[Memory::position]);//Memory::DMemory[Memory::position];
			inT1 = Memory::DMemory[Memory::position];
			inT1 = inT1 << 24 >> 24;
			Register::reg[Instruction::rt] = inT1;
		}
		detectWrite2Zero("I");
		Iimm = (int)Instruction::immediate;
		Ipos = Irs + Iimm;
    	detectNumberOverflow(Irs,Iimm,Ipos);
		//detectNumberOverflow("I", false, true);
	}
	else if(op == "lbu") {
		SignedImmediate();
		Memory::position = Register::reg[Instruction::rs] + Instruction::immediate; // need detection
		//if(detectMemoryOverflow(0)) return;
		//if(detectDataMisaaligned(0)) return;
		isMemoryOverflow = detectMemoryOverflow(0);
		isDataMisaaligned = detectDataMisaaligned(0);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			Register::reg[Instruction::rt] = Memory::DMemory[Memory::position];//<< 24 >> 24;
		}
		detectWrite2Zero("I");
		Iimm = (int)Instruction::immediate;
		Ipos = Irs + Iimm;
    	detectNumberOverflow(Irs,Iimm,Ipos);
		//detectNumberOverflow("I", false, true);
	}
	else if(op == "sw") {
		SignedImmediate();
		Memory::position = Register::reg[Instruction::rs] + Instruction::immediate; // need detection
		//if(detectMemoryOverflow(3)) return;
		//if(detectDataMisaaligned(3)) return;
		isMemoryOverflow = detectMemoryOverflow(3);
		isDataMisaaligned = detectDataMisaaligned(3);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			//Memory::DMemory[Memory::position] = Register::reg[Instruction::rt] >> 24;
			//Memory::DMemory[Memory::position+1] = Register::reg[Instruction::rt] << 8 >> 24;
			//Memory::DMemory[Memory::position+2] = Register::reg[Instruction::rt] << 16 >> 24;
			//Memory::DMemory[Memory::position+3] = Register::reg[Instruction::rt] << 24 >> 24;
			Memory::DMemory[Memory::position] = Register::reg[Instruction::rt] >> 24;
			Memory::DMemory[Memory::position + 1] = (Register::reg[Instruction::rt] >> 16) & 0xff;
			Memory::DMemory[Memory::position + 2] = (Register::reg[Instruction::rt] >> 8) & 0xff;
			Memory::DMemory[Memory::position + 3] = (Register::reg[Instruction::rt]) & 0xff;
		}
		Iimm = (int)Instruction::immediate;
		Ipos = Irs + Iimm;
    	detectNumberOverflow(Irs,Iimm,Ipos);
		//detectNumberOverflow("I", false, true);
	}
	else if(op == "sh") {
		SignedImmediate();
		Memory::position = Register::reg[Instruction::rs] + Instruction::immediate; // need detection
		//if(detectMemoryOverflow(1)) return;
		//if(detectDataMisaaligned(1)) return;
		isMemoryOverflow = detectMemoryOverflow(1);
		isDataMisaaligned = detectDataMisaaligned(1);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			Memory::DMemory[Memory::position] = (Register::reg[Instruction::rt] >> 8) & 0xff;//Register::reg[Instruction::rt] << 16 >> 24;
			Memory::DMemory[Memory::position+1] = Register::reg[Instruction::rt]; //<< 24 >> 24;
		}
		Iimm = (int)Instruction::immediate;
		Ipos = Irs + Iimm;
    	detectNumberOverflow(Irs,Iimm,Ipos);
		//detectNumberOverflow("I", false, true);
	}
	else if(op == "sb") {
		SignedImmediate();
		Memory::position = Register::reg[Instruction::rs] + Instruction::immediate; // need detection
		//if(detectMemoryOverflow(0)) return;
		//if(detectDataMisaaligned(0)) return;
		isMemoryOverflow = detectMemoryOverflow(0);
		isDataMisaaligned = detectDataMisaaligned(0);
		if(!isMemoryOverflow && !isDataMisaaligned) {
			Memory::DMemory[Memory::position] = Register::reg[Instruction::rt]; //<< 24 >> 24;
		}
		Iimm = (int)Instruction::immediate;
		Ipos = Irs + Iimm;
    	detectNumberOverflow(Irs,Iimm,Ipos);
		//detectNumberOverflow("I", false, true);
	}
	else if(op == "lui") {
		UnsignedImmediate();
		Register::reg[Instruction::rt] = Instruction::immediate << 16;
		detectWrite2Zero("I");
	}
	else if(op == "andi") {
		UnsignedImmediate();
		Register::reg[Instruction::rt] = Register::reg[Instruction::rs] & Instruction::immediate;
		detectWrite2Zero("I");
	}
	else if(op == "ori") {
		UnsignedImmediate();
		Register::reg[Instruction::rt] = Register::reg[Instruction::rs] | Instruction::immediate;
		detectWrite2Zero("I");
	}
	else if(op == "nori") {
		UnsignedImmediate();
		Register::reg[Instruction::rt] = ~(Register::reg[Instruction::rs] | Instruction::immediate);
		detectWrite2Zero("I");
	}
	else if(op == "slti") {
		SignedImmediate();
		Register::reg[Instruction::rt] = ((int)Register::reg[Instruction::rs] < (int)Instruction::immediate) ? 1 : 0;
		detectWrite2Zero("I");
	}
	else if(op == "beq") {
		SignedImmediate();
		if(Register::reg[Instruction::rs]==Register::reg[Instruction::rt]) {
			Register::PC += (4 + (Instruction::immediate << 2));
			return;
		}
	}
	else if(op == "bne") {
		SignedImmediate();
		if(Register::reg[Instruction::rs]!=Register::reg[Instruction::rt]) {
			Register::PC += (4 + (Instruction::immediate << 2));
			return;
		}
	}
	else { //if(op == "bgtz")
		SignedImmediate();
		if((int)Register::reg[Instruction::rs] > 0) {
			Register::PC += (4 + (Instruction::immediate << 2));
			return;
		}
	}

	Register::PC += 4;
}

void J_format(string op) {
	if(op=="jal") Register::reg[31] = Register::PC + 4;
    Address();
    Register::PC = ((Register::PC + 4) >> 28 << 28) | (Memory::address << 2);
}