#include "load.h"

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

