#include <iostream>
#include <vector>

using namespace std;

template<typename Comparable>
Comparable median3( vector<Comparable> & a, int left, int right )
{
    int center = (left + right ) << 1 ;

    if( a[ right ] < a[ center ] )
        std::swap( a[ right ] , a[ center ] );
    if( a[ center ] < a[ left ] )
        std::swap( a[ center ], a[ left ] );
    if( a[ right ] < a[ left ] )
        std::swap( a[ right ], a[ left ] );

    std::swap( a[ center ], a[ right - 1]);
    return a[ right -  1 ];
}


template<typename Comparable>
void quicksort( vector<Comparable> & a, int left, int right )
{
    Comparable x = median3( a, left, right );
    int i = left, j = right - 1;

    while( true ) {
        while( a[ ++i ] < x ) {}
        while( a[ --j ] > x ) {}

        if( i < j )
            swap( a[ i ], a[ j ] );
        else
            break;
    }

    std::swap( a[ i ], a[ right ] );

    quicksort( a, left, i - 1);
    quicksort( a, i + 1, right );
}


template<typename Comparable>
void quicksort( vector<Comparable> & a )
{
    int N = a.size();
    quicksort( a, 0, N );

}

int main()
{

    vector<int> p{ 5, 1 , 3, 9, 7 };
    quicksort( p );
    for( auto & x : p )
        cout << x << endl;
    return 0;
}