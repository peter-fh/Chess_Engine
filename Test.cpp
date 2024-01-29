#include <iostream>
#include <bitset>
using namespace std;

int main(){
    uint16_t num = 0b1111;
    cout << bitset<4>(num & (-num));

}