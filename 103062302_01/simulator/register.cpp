#include "Register.h"
#include<bitset>

string Register::2Hex() {
    string Hex = "";
    bitset<32> b; /* every four bits a group to form Hex */
    int sum,power;
    
    b = value;
    for(int i=0; i<8; i++) {
        sum=0; power=1;
        for(int j=i*4; j<(i+1)*4; j++) {
            sum += power * b[j];
            power *= 2; /* 1->2->4->8->... */
        }
        
        if(sum >= 10) {
            switch(sum) {
                case 10:
                    Hex = 'A' + Hex;
                    break;
                case 11:
                    Hex = 'B' + Hex;
                    break;
                case 12:
                    Hex = 'C' + Hex;
                    break;
                case 13:
                    Hex = 'D' + Hex;
                    break;
                case 14:
                    Hex = 'E' + Hex;
                    break;
                case 15:
                    Hex = 'F' + Hex;
                    break;
            }
         }
         else Hex = (char)(sum+'0') + Hex;
    }
    Hex = "0x" + Hex;
    return Hex;    
}

