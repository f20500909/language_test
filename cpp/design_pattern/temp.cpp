#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <bitset>
using namespace std;
uint32_t reverseBits(uint32_t n) {
    uint32_t left=0x80000000;
    uint32_t right=0x1;
    while(left>right){
        if((left&n)!=(right&n)){
            n^=left;
            n^=right;
        }
        left=left>>1;
        right=right<<1;
        cout<<bitset<32>(n)<<endl;
    }

    cout<<"==========="<<endl;
    return n;

}

int main(void)
{
    cout<<bitset<32>(reverseBits(0b00000010100101000001111010011100))<<endl;
    cout<<bitset<32>(0b00111001011110000010100101000000)<<endl;
    cout<<bitset<32>(0b00000010100101000001111010011100)<<endl;

    return 0;
}

