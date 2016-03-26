#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "resolve.h"
#include "translate.h"
#include "instruction.h"

FILE *iimage, *dimage, *error_dump, *snapshot;
unsigned iimageLen, dimageLen;
char *iimageBuffer, *dimageBuffer;

void Open() {
    iimage = fopen("iimage.bin", "rb");
    dimage = fopen("dimage.bin", "rb");
    error_dump = fopen("error_dump.rpt", "wb");
    snapshot = fopen("snapshot.rpt", "wb");

    fseek(iimage, 0, SEEK_END);
    fseek(dimage, 0, SEEK_END);

    iimageLen = (unsigned)ftell(iimage);
    dimageLen = (unsigned)ftell(dimage);

    // move to the beginning of the file
    rewind(iimage);
    rewind(dimage);

    // allocate memory
    iimageBuffer = new char[iimageLen];
    dimageBuffer = new char[dimageLen];

    fread(iimageBuffer, 1, iimageLen, iimage);
    fread(dimageBuffer, 1, dimageLen, dimage);

    fclose(iimage);
    fclose(dimage);
}

void DImg() {
    unsigned tmp=0, num=0, index=0;

    for(int i=0; i<4; i++) {
        tmp = (tmp<<8) + (unsigned char)dimageBuffer[i];
    }
    Register::reg[29] = tmp; //$SP

    for(int i=4; i<8; i++) {
        num = (num<<8) + (unsigned char)dimageBuffer[i];
    }

    for(int i=8; i<8+4*num; i++) {
        Memory::DMemory[index++] = dimageBuffer[i];
    }
}

void IImg() {
    unsigned tmp=0, num=0, index=0;

    for(int i=0; i<4; i++) {
        tmp = (tmp<<8) + (unsigned char)iimageBuffer[i];
    }
    Register::PC = tmp;

    for(int i=4; i<8; i++) {
        num = (num<<8) + (unsigned char)iimageBuffer[i];
    }

    index = Register::PC;
    for(int i=8; i<8+4*num; i++) {
        Memory::IMemory[index++] = iimageBuffer[i];
    }
}

void errorDump() {
    if (Terminal::write2Zero) {
        fprintf(error_dump, "In cycle %d: Write $0 Error\n", Register::cycle);
        //printf("cycle %d: Write $0 Error\n", Register::cycle);
    }
    if (Terminal::numberOverflow) {
        fprintf(error_dump, "In cycle %d: Number Overflow\n", Register::cycle);
        //printf("cycle %d: Number Overflow\n", Register::cycle);
    }
    if (Terminal::memoryOverflow) {
        fprintf(error_dump, "In cycle %d: Address Overflow\n", Register::cycle);
        //printf("cycle %d: Address Overflow\n", Register::cycle);
    }
    if (Terminal::dataMisaaligned) {
        fprintf(error_dump, "In cycle %d: Misalignment Error\n", Register::cycle);
        //printf("cycle %d: Misalignment Error\n", Register::cycle);
    }
}

void snapShot() {
    fprintf(snapshot, "cycle %d\n", Register::cycle);
    //printf("Register::cycle %d\n", Register::cycle);
    for(int i=0; i<32; i++) {
        fprintf(snapshot, "$%02d: 0x", i);
        fprintf(snapshot, "%08X\n", Register::reg[i]);
        //printf("$%02d: 0x", i);
        //printf("%08X\n", Register::reg[i]);
    }
    fprintf(snapshot, "PC: 0x%08X\n\n\n", Register::PC);
    //printf("PC: 0x%08X\n\n\n", Register::PC);
    Register::cycle++;
}

int main() {
    Open();
    DImg();
    IImg();

    Terminal::halt = false;
    while(!Terminal::halt) {
    	Terminal::write2Zero = false;
    	Terminal::numberOverflow = false;
    	Terminal::memoryOverflow = false;
    	Terminal::dataMisaaligned = false;
    	snapShot();
    	Assembly();
    	errorDump();
    }
 
    return 0;
}
