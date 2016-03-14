#include "translate.h"

void 2Assembly(vector<int> Word) {
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
					
					break;
				case 33:
					
					break;
				case 34:
					
					break;
				case 36:
					
					break;
				case 37:
					
					break;
				case 38:
					
					break;
				case 39:
					
					break;
				case 40:
					
					break;
				case 42:
					
					break;
				case 0:
					
					break;
				case 2:
					
					break;
				case 3:
					
					break;
				case 8:
					
					break;
			}
			break;
		case 8:
			
			break;
		case 9:
			
			break;
		case 35:
			
			break;
		case 33:
			
			break;
		case 37:
			
			break;
		case 32:
			
			break;
		case 36:
			
			break;
		case 43:
			
			break;
		case 41:
			
			break;
		case 40:
			
			break;
		case 15:
			
			break;
		case 12:
			
			break;
		case 13:
			
			break;
		case 14:
			
			break;
		case 10:
			
			break;
		case 4:
			
			break;
		case 5:
			
			break;
		case 7:
			
			break;
		case 2:
			
			break;
		case 3:
			
			break;
		case 63:
			AllRegister::PC.value = 0xFFFF; /* halt */
	}
}

