#include "resolve.h"

void Opcode() {
	op = IMemory[PC];
	op = op >> 2 << 26 >> 26;
}

void RsRtRd() {
	unsigned t1 = IMemory[PC], t2 = IMemory[PC+1];
	t1 = t1 << 30 >> 27;
	t2 = t2 << 24 >> 29;
	rs = t1 + t2;

	rt = IMemory[PC+1];
	rt = rt << 27 >> 27;

	//if(rd!=NULL) {
		rd = IMemory[PC+2];
		rd = rd << 24 >> 27;
	//}
}

void Shamt(unsigned *shamt) {
	unsigned t1 = IMemory[PC+2], t2 = IMemory[PC+3];
	t1 = t1 << 29 >> 27;
	t2 = t2 >> 6 << 30 >> 30;
	*shamt = t1 + t2;
}

void Func() {
	func = IMemory[PC+3];
	func = func << 26 >> 26;
}

void UnsignedImmediate(unsigned *immediate) {
	unsigned t1 = IMemory[PC+2], t2 = IMemory[PC+3];
	t1 = t1 << 24 >> 16;
	t2 = t2 << 24 >> 24;
	*immediate = t1 + t2;
}

void SignedImmediate(unsigned *immediate) {
	unsigned t1 = IMemory[PC+2], t2 = IMemory[PC+3];
	t1 = t1 << 24 >> 16;
	t2 = t2 << 24 >> 24;
	*immediate = (t1 + t2) << 16 >> 16;
}

void Address(unsigned *address) {
	unsigned t1 = IMemory[PC], t2 = IMemory[PC+1], t3 = IMemory[PC+2], t4 = IMemory[PC+3];
	t1 = t1 << 30 >> 6;
	t2 = t2 << 24 >> 8;
	t3 = t3 << 24 >> 16;
	t4 = t4 << 24 >> 24;
	*address = t1 + t2 + t3 + t4;
}
