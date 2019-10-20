// for spwm generation.
#ifdef __linux__
#include "SPWM_int.h"
#include <iostream>
#include <fstream>
using namespace std;
    
int main()
{
    uint32_t freq_ = 50;
    bool generalTest;
    cin >> generalTest;
    if (!generalTest)
    {
        ofstream fout("./result.dat", std::ios_base::out);
        int32_t data;
        fout << "data = [";
        for(uint64_t i = 0; i < freq / d_freq; i ++){
            SPWM_ISR();
            fout << TA2CCR2 << ",";
        }
        fout << ']' << endl;
        fout << "DMA1SZ = " << freq / d_freq<< endl;
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