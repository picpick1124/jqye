#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#define NOT_FOUND -1
using namespace std;


long long gcd (long long m, long long n)
{
    while(n != 0) {
        long long rem = m % n;
        m = n;
        n = rem;
    }
    return m;
}

int main()
{
}