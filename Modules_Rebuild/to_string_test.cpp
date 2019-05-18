#ifdef __linux__
#include "to_string.h"
#include "iostream"
int main(){
    float data;
    while(std::cin >> data){
        std::cout << to_string(data) << std::endl;
    }
    return 0;
}
#endif