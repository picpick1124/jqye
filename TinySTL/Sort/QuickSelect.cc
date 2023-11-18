#include <iostream>
#include <vector>

using namespace std;

template<typename Comparable>
void insertionSort( vector<Comparable> & a )
{
    for( int i = 1; i < a.size(); ++i ) {
        Comparable tmp = std::move( a[ i ] );

        int p;
        for( p = i; p > 0 && tmp < a[ p - 1 ]; --p ) {
            a[ p ] = std::move( a[ p - 1 ] );
        }

        a[ p ] = std::move( tmp );
    }
}

template<typename Comparable>
const Comparable & media3( vector<Comparable> & a, int left, int right )
{
    int center = ( left + right ) / 2;

    if( a[ left ] < a[ center ] )
        std::swap( a[ left ], a[ center ] );
    if( a[ left ] < a[ right ] )
        std::swap( a[ left ], a[ right ] );
    if( a[ center ] < a[ right ] )
        std::swap( a[ center ], a[ right ] );

    std::swap( a[ center ], a[ right - 1 ] );
    return a[ right - 1 ];
}

template<typename Comparable>
void quickselect( vector<Comparable> & a, int left, int right, int k)
{
    if( left + 10 <= right ) {
        const Comparable & pivot = media3( a, left, right );

        int i = left, j = right - 1;

        for( ; ; ) {
            while( a[ ++i ] < pivot ) { }
            while( pivot < a[ --j ] ) { }
            if( i < j )
                std::swap( a[ i ], a[ j ] );
            else
                break;
        }

        std::swap( a[ i ], a[ right - 1 ] );

        if( k <= i ) {
            quickselect( a, left, i, k );
        } else if( k > i) {
            quickselect( a, i + 1, right, k );
        }
    }
    else
        insertionSort( a );
}




template<typename Comparable>
void quickselect( vector<Comparable> & a, int k )
{
    quickselect( a, 0, a.size() -1, k );
}


int main()
{
    vector<int> vec { 5, 4, 2, 9, 1, 3, 7, 9, 4, 8};
    quickselect( vec, 5 );

    int a = vec[ 4 ];
    cout << a << endl;

    return 0;
}


