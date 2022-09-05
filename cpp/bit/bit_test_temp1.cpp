#include <iostream>
#include <cassert>
using namespace std;



#define MAKEINT32(a, b)  ((uint32_t)(((uint16_t)((uint32_t)(a) & 0xffff)) | ((uint32_t)((uint16_t)((uint32_t)(b) & 0xffff))) << 16))

int makeInt32(int lo, int hi)
{
    return MAKEINT32(lo, hi);
}



int main() {
    std::cout<<(makeInt32(0xff00,0x00));


}