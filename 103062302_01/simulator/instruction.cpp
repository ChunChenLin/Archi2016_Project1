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
	else if(op == "srl") {

	}
	else if(op == "sra") AllRegister::reg[rd].value = AllRegister::reg[rt].value >> shamt;
	else if(op == "jr") AllRegister::PC.value = AllRegister::reg[rs].value; return; /* don't need to plus 4*/

	AllRegister::PC.value += 4; /* need to plus 4 immediately*/ 
}

void I_format(string op, int rs, int rt, int imm) {
	if(op == "addi") AllRegister::reg[rt].value = AllRegister.reg[rs].value + imm;
	else if(op == "addiu") unsigned int ui = imm; AllRegister::reg[rt].value = AllRegister::reg[rs].value + ui;
	else if(op == "lw")
	else if(op == "lh")
	else if(op == "lhu")
	else if(op == "lb")
	else if(op == "lbu")
	else if(op == "sw")
	else if(op == "sh")
	else if(op == "sb")
	else if(op == "andi")
	else if(op == "ori")
	else if(op == "nori")
	else if(op == "slti")
	else if(op == "beq") return;
	else if(op == "bne") return;

	else if(op == "lui") return;
	else if(op == "bgtz") return;

	CPURegister::PC.value += 4;
}

void J_format(string op, unsigned int addr) {
	if(op == "jal") AllRegister::reg[31].value = AllRegister::PC.value + 4;
	
}  


