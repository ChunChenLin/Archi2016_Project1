#include "instruction.h"
#include "global.h"

/* look up Appendix A */

void R_format(string op, int rs, int rt, int rd, int shamt) { /* func is merged into op */
    if(op == "add")	AllRegister::reg[rd].value = AllRegister::reg[rs].value + AllRegister::reg[rt].value;
	else if(op == "addu") /* haven't implement */
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
	}
	else if(op == "sra") AllRegister::reg[rd].value = AllRegister::reg[rt].value >> shamt;
	else if(op == "jr") AllRegister::PC.value = AllRegister::reg[rs].value; return; /* don't need to plus 4*/

	AllRegister::PC.value += 4; /* need to plus 4 immediately*/ 
}

void I_format(string op, int rs, int rt, int imm) {
	if(op == "addi") AllRegister::reg[rt].value = AllRegister.reg[rs].value + imm;
	else if(op == "addiu") unsigned int ui = imm; AllRegister::reg[rt].value = AllRegister::reg[rs].value + ui;
	else if(op == "lw") {
		/* $t = 4 bytes from Memory[$s + C(signed)] */
	}
	else if(op == "lh") {
		/* $t = 2 bytes from Memory[$s + C(signed)], signed */
	}
	else if(op == "lhu") {
		/* $t = 2 bytes from Memory[$s + C(signed)], unsigned */
	}
	else if(op == "lb") {
		/* $t = Memory[$s + C(signed)], signed */
	}
	else if(op == "lbu") {
		/* $t = Memory[$s + C(signed)], unsigned */
	}
	else if(op == "sw")
	else if(op == "sh")
	else if(op == "sb")
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

void J_format(string op, unsigned int addr) {
	if(op == "jal") AllRegister::reg[31].value = AllRegister::PC.value + 4;
	
}  


