#include "global.h"

unsigned Register::reg[32];
unsigned Register::PC;
unsigned Register::cycle;

unsigned char Memory::DMemory[1024];
unsigned char Memory::IMemory[1024];
unsigned Memory::address;
unsigned Memory::position;

unsigned Instruction::op;
unsigned Instruction::rs;
unsigned Instruction::rt;
unsigned Instruction::rd;
unsigned Instruction::func;
unsigned Instruction::shamt;
unsigned Instruction::immediate;

bool Terminal::halt;
bool Terminal::write2Zero;
bool Terminal::numberOverflow;
bool Terminal::memoryOverflow;
bool Terminal::dataMisaaligned;


