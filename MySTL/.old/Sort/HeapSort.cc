#include <iostream>
#include <vector>

using namespace std;


inline int leftChild( int i )
{
    return 2 * i + 1;
}

template<typename Comparable>
void procDown( vector<Comparable> & a, int i, int n )
{
    int child;
    Comparable tmp;

    for( tmp = std::move( a[ i ] ); leftChild( i ) < n; i = child ) {
        child = leftChild( i );
        if( child != n - 1 && a[ child ] < a[ child + 1 ] )
            ++child;
        if( tmp < a[ child ] )
            a[ i ] = std::move( a[ child ] );
        else
            break;
    }
    a[ i ] = std::move( tmp );

    // for( ; child < a.size() && tmp < a[ child ]; i = child ) {
    //     if( a[ child ] != j && a[ child ] < a[ child + 1 ] )
    //         ++child;
    //     a[ i ] = std::move( a[ child ] );
    // }
    // a[ i ] = std::move( tmp );
}


template<typename Comparable>
void heapsort( vector<Comparable> & a )
{
    for( int i = a.size() / 2  - 1; i >= 0; --i )
        procDown( a, i, a.size() );

    for( int j = a.size() - 1; j > 0; --j ) {
        std::swap( a[ 0 ], a[ j ]);
        procDown( a, 0, j );
    }
}

int main()
{
    vector<int> vec { 5, 4, 2, 9, 1, 3, 7, 8};
    for( auto x : vec )
        cout << x << ' ';
    cout << '\n';

    heapsort( vec );

    for( auto x : vec )
        cout << x << ' ';
    cout << '\n';

    return 0;
}
