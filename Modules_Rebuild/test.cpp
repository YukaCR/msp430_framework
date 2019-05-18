#ifdef __linux__
#include "DAC7612U.h"
#include <iostream>
#include <fstream>
using namespace std;
    
int main()
{
    bool generalTest;
    cin >> generalTest;
    if (!generalTest)
    {
        ofstream fout("./result.dat", std::ios_base::out);
        int32_t data;
        fout << "data = [";
        ChangeSqProp(1000,0.5); 
        for (uint32_t i = 0; i < DMA1SZ; i+=2)
        {
            fout << *(uint16_t*)(DMA_SPI_Buffer + i) << ',';
        }
        fout << ']' << endl;
        fout << "DMA1SZ = " << (DMA1SZ + 1) / 2 << endl;
    }
    else
    {
        int32_t data;
        while (cin >> data)
        {
            cout << Sin(data) << endl;
        }
    }
    return 0;
}
#endif