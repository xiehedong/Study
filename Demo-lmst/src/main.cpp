#include<iostream>

int i, gi;
void* address;

void* buildCode() {
    char* code = new char(16);
    char* pMov = code;
    char* pJmp = code + 10;
    char* pAddress;

    //mov gi, 18
    pMov[0] = 0xc7;
    pMov[1] = 0x05;
    pAddress = pMov + 2;
    *((int*)pAddress) = (int)&gi;
    *((int*)(pAddress + 4)) = 18;
    /*int dz = (int)&gi;
    int num = 18;
    memcpy_s(pAddress, 4, &dz, 4);
    memcpy_s(pAddress + 4, 4, &num, 4);*/

    //jmp address
    pJmp[0] = 0xff;
    pJmp[1] = 0x25;
    *((int*)&pJmp[2]) = (int)&address;
    /*dz = (int)&address;
    memcpy_s(pJmp + 2, 4, &dz, 4);*/

    return code;
}

int main()
{
    void* code = buildCode();
    
    __asm {
        mov address, offset _lb1
    }

    gi = 12;
    printf("gi = %d\n", gi);

    __asm jmp code

    gi = 13;

_lb1:
    printf("gi = %d\n", gi);

    system("pause");

    delete code;
    code = nullptr;
    return 0;
}

