#include "translate.h"

unsigned int C_Iformat(std::vector<int> Word, bool Signed) {
	int sum=0, power=1, neg[16];
	if(Signed) {
		/* make sure it's negative */
		if(Word[15]==1) { 
			for(int i=0; i<16; i++) neg[i] = !Word[i];
			/* 2's complement */
			int carry = 1, tmp;
			for(int i=0; i<16; i++) {
				tmp = (neg[i]+carry) % 2;
				carry = (neg[i]+carry) / 2;
				neg[i] = tmp;
				sum += (neg[i] * power);
				power *= 2;
			}
			sum *= (-1);
			return sum;
		}
	} else {
		for(int i=0; i<16; i++){
			sum += (Word[i] * power);
			power *= 2;
		}
		return sum;
	}
	
}

void Assembly(vector<int> Word) {
    int op=0,rs=0,rt=0,rd=0,shamt=0,func=0;
    int power;

    /* 31~26 for opcode */
    power = 1;
    for(int i=0;i<6;i++) { 
    	op += Word[26+i]*power;
    	power *= 2;
    }

    /* 25~21 for rs*/
    power = 1;
    for(int i=0;i<5;i++) {
    	rs += Word[21+i]*power;
    	power *= 2;
    }

    /* 20~16 for rt */
    power = 1;
    for(int i=0;i<5;i++) {
    	rt += Word[21+i]*power;
    	power *= 2;
    }

    /* 15~11 for rd */
    power = 1;
    for(int i=0;i<5;i++) {
    	rd += Word[21+i]*power;
    	power *= 2;
    }

    /* 10~6 for shamt */
	power = 1;
	for(int i=0; i<5; i++){
		shamt += (Word[6+i]*power);
		power *= 2;
	}

	/* 5~0 for func */
	power = 1;
	for(int i=0; i<6; i++){
		funct += (Word[i]*power);
		power *= 2;
	}

	/* judge and call instruction func */
	switch (op) {
		case 0:
			switch (func) {
				case 32:
					R_format("add", rs, rt, rd, shamt);
					break;
				case 33:
					R_format("addu", rs, rt, rd, shamt);
					break;
				case 34:
					R_format("sub", rs, rt, rd, shamt);
					break;
				case 36:
					R_format("and", rs, rt, rd, shamt);
					break;
				case 37:
					R_format("or", rs, rt, rd, shamt);
					break;
				case 38:
					R_format("xor", rs, rt, rd, shamt);
					break;
				case 39:
					R_format("nor", rs, rt, rd, shamt);
					break;
				case 40:
					R_format("nand", rs, rt, rd, shamt);
					break;
				case 42:
					R_format("slt", rs, rt, rd, shamt);
					break;
				case 0:
					R_format("sll", rs, rt, rd, shamt);
					break;
				case 2:
					R_format("srl", rs, rt, rd, shamt);
					break;
				case 3:
					R_format("sra", rs, rt, rd, shamt);
					break;
				case 8:
					R_format("jr", rs, rt, rd, shamt);
					break;
			}
			break;
		case 8:
			I_format("addi", rs, rt, C_Iformat(Word, true));
			break;
		case 9:
			I_format("addiu", rs, rt, C_Iformat(Word, false));
			break;
		case 35:
			I_format("lw", rs, rt, C_Iformat(Word, true));
			break;
		case 33:
			I_format("lh", rs, rt, C_Iformat(Word, true));
			break;
		case 37:
			I_format("lhu", rs, rt, C_Iformat(Word, true));
			break;
		case 32:
			I_format("lb", rs, rt, C_Iformat(Word, true));
			break;
		case 36:
			I_format("lbu", rs, rt, C_Iformat(Word, true));
			break;
		case 43:
			I_format("sw", rs, rt, C_Iformat(Word, true));
			break;
		case 41:
			I_format("sh", rs, rt, C_Iformat(Word, true));
			break;
		case 40:
			I_format("sb", rs, rt, C_Iformat(Word, true));
			break;
		case 15:
			I_format("lui", rs, rt, C_Iformat(Word, true));
			break;
		case 12:
			I_format("andi", rs, rt, C_Iformat(Word, false));
			break;
		case 13:
			I_format("ori", rs, rt, C_Iformat(Word, false));
			break;
		case 14:
			I_format("nori", rs, rt, C_Iformat(Word, false));
			break;
		case 10:
			I_format("slti", rs, rt, C_Iformat(Word, true));
			break;
		case 4:
			I_format("beq", rs, rt, C_Iformat(Word, true));
			break;
		case 5:
			I_format("bne", rs, rt, C_Iformat(Word, true));
			break;
		case 7:
			I_format("bgtz", rs, rt, C_Iformat(Word, true));
			break;
		case 2:
			J_format("j", unsigned int addr)
			break;
		case 3:
			J_format("jal", unsigned int addr)
			break;
		case 63:
			AllRegister::PC.value = 0xFFFF; /* halt */
	}
}

