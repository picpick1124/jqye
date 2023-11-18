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
void quicksort( vector<Comparable> & a, int left, int right )
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

        quicksort( a, left, i - 1 );
        quicksort( a, i + 1, right );
    }
    else
        insertionSort( a );
}




template<typename Comparable>
void quicksort( vector<Comparable> & a )
{
    quicksort( a, 0, a.size() -1 );
}


int main()
{
    vector<int> vec;
    int tmp = 0;
    while( cin >> tmp ) vec.push_back( std::move( tmp ) );
    quicksort( vec );

    for( auto x : vec )
        cout << x << endl;

    return 0;
}
