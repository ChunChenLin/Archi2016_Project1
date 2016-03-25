#include "resolve.h"

void Opcode() {
	Instruction::op = Memory::IMemory[Register::PC];
	Instruction::op = Instruction::op >> 2 << 26 >> 26;
}

void RsRtRd() {
	unsigned t1 = Memory::IMemory[Register::PC], t2 = Memory::IMemory[Register::PC+1];
	t1 = t1 << 30 >> 27;
	t2 = t2 << 24 >> 29;
	Instruction::rs = t1 + t2;

	Instruction::rt = Memory::IMemory[Register::PC+1];
	Instruction::rt = Instruction::rt << 27 >> 27;

	Instruction::rd = Memory::IMemory[Register::PC+2];
	Instruction::rd = Instruction::rd << 24 >> 27;

}

void Shamt() {
	unsigned t1 = Memory::IMemory[Register::PC+2], t2 = Memory::IMemory[Register::PC+3];
	t1 = t1 << 29 >> 27;
	t2 = t2 >> 6 << 30 >> 30;
	Instruction::shamt = t1 + t2;
}

void Func() {
	Instruction::func = Memory::IMemory[Register::PC+3];
	Instruction::func = Instruction::func << 26 >> 26;
}

void UnsignedImmediate() {
	unsigned t1 = Memory::IMemory[Register::PC+2], t2 = Memory::IMemory[Register::PC+3];
	t1 = t1 << 24 >> 16;
	t2 = t2 << 24 >> 24;
	Instruction::immediate = t1 + t2;
}

void SignedImmediate() {
	unsigned t1 = Memory::IMemory[Register::PC+2], t2 = Memory::IMemory[Register::PC+3];
	t1 = t1 << 24 >> 16;
	t2 = t2 << 24 >> 24;
	Instruction::immediate = int(int(t1 + t2) << 16 >> 16);
}

void Address() {
	unsigned t1 = Memory::IMemory[Register::PC], t2 = Memory::IMemory[Register::PC+1], t3 = Memory::IMemory[Register::PC+2], t4 = Memory::IMemory[Register::PC+3];
	t1 = t1 << 30 >> 6;
	t2 = t2 << 24 >> 8;
	t3 = t3 << 24 >> 16;
	t4 = t4 << 24 >> 24;
	Memory::address = t1 + t2 + t3 + t4;
}
