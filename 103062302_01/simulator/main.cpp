#include<iostream>
#include<fstream>
#include<string>
#include<bitset>

using namespace std;

int main() {
    int word[32];
    char ch;
    bitset<8> bs;    

    /* Read iimage.bin */
    ifstream fin("../testcase/iimage.bin",ios::in | ios::binary);
    if(!fin) cout<<"Can't load iimage.bin\n";
    
    /* until the end of the file */
    while(!fin.eof()) {
        
    }
    fin.close();

    /* Read dimage.bin */
    ifstream fin("../testcase/dimage.bin",ios::in | ios::binary);
    if(!fin) cout<<"Can't load dimage.bin\n";

    fin.close();
}
