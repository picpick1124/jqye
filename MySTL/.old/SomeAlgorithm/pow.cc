#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#define NOT_FOUND -1
using namespace std;

int pow(int base, int b)
{
    if(b == 0)
        return 1;
    if(b%2==0) {
        return pow(base*base, b/2);
    } else {
        return pow(base*base,b/2)*base;
    }
}

int main()
{
}