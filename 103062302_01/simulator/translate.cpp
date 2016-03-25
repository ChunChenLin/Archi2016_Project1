#include "translate.h"

void Assembly() {

	/* judge and call instruction func */
	Opcode();
	switch (Instruction::op) {
		case 0: //R format
			Func();
			switch (Instruction::func) {
				case 32:
					R_format("add");
					break;
				case 33:
					R_format("addu");
					break;
				case 34:
					R_format("sub");
					break;
				case 36:
					R_format("and");
					break;
				case 37:
					R_format("or");
					break;
				case 38:
					R_format("xor");
					break;
				case 39:
					R_format("nor");
					break;
				case 40:
					R_format("nand");
					break;
				case 42:
					R_format("slt");
					break;
				case 0:
					R_format("sll");
					break;
				case 2:
					R_format("srl");
					break;
				case 3:
					R_format("sra");
					break;
				case 8:
					R_format("jr");
					break;
			}
			break;
		case 8:
			I_format("addi");
			break;
		case 9:
			I_format("addiu");
			break;
		case 35:
			I_format("lw");
			break;
		case 33:
			I_format("lh");
			break;
		case 37:
			I_format("lhu");
			break;
		case 32:
			I_format("lb");
			break;
		case 36:
			I_format("lbu");
			break;
		case 43:
			I_format("sw");
			break;
		case 41:
			I_format("sh");
			break;
		case 40:
			I_format("sb");
			break;
		case 15:
			I_format("lui");
			break;
		case 12:
			I_format("andi");
			break;
		case 13:
			I_format("ori");
			break;
		case 14:
			I_format("nori");
			break;
		case 10:
			I_format("slti");
			break;
		case 4:
			I_format("beq");
			break;
		case 5:
			I_format("bne");
			break;
		case 7:
			I_format("bgtz");
			break;
		case 2:
			J_format("j");
			break;
		case 3:
			J_format("jal");
			break;
		case 63:
			/* halt */
			Terminal::halt = true;
			break;
	}
}

