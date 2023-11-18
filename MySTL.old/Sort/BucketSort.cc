#include <iostream>
#include <vector>

using namespace std;
template<typename Comparable>
Comparable findMax( vector<Comparable> & a )
{
    Comparable tmp = a[ 0 ];
    for( int i = 0; i < static_cast<int>(a.size()); ++i ) {
        if( tmp < a[ i ] )
            tmp = a[ i ];
    }

    return tmp;
}

template<typename Comparable>
vector<Comparable> bucketsort( vector<Comparable> & a)
{
    int max = findMax( a );
    vector<Comparable> b ( max );

    for( int i = 0; i < static_cast<int>(a.size()); ++i ) {
        int tmp = a[ i ];
        b[ tmp ] += 1;
    }

    return b;
}

int main()
{
    vector<int> vec { 77, 5, 4, 2, 9, 1, 3, 7, 9, 4, 8, 88, 0, 11, 32};
    vector<int> b = bucketsort<int>( vec );
    for( int i = 0; i < static_cast<int>(b.size()) ; ++i ) {
        if( b[ i ] > 0 )
            cout << b[ i ] << endl;
    }
    return 0;
}


