#include <iostream>
#include <vector>

using namespace std;



template<typename Comparable>
void merge( vector<Comparable> & a, vector<Comparable> & tmpArray, int leftPos,
            int rightPos, int rightEnd)
{
    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;

    while( leftPos <= leftEnd && rightPos <= rightEnd ) {
        if( a[ leftPos ] <= a[ rightPos ] )
            tmpArray[ tmpPos++ ] = std::move( a[ leftPos++ ] );
        if( a[ rightPos ] <= a[ leftPos ] )
            tmpArray[ tmpPos++ ] = std::move( a[ rightPos++ ] );
    }

    while( leftPos <= leftEnd )
        tmpArray[ tmpPos++ ] = std::move( a[ leftPos++ ] );

    while( rightPos <= rightEnd )
        tmpArray[ tmpPos++ ] = std::move( a[ rightPos++ ] );

    for( int i = 0; i < numElements; ++i, --rightEnd )
        a[ rightEnd ] = std::move( tmpArray[ rightEnd ] );

    //复制的另一种实现
    // tmpPos -= 1;
    // for( int i = 0; i < numElements; ++i, --rightEnd )
    //     a[ rightEnd ] = std::move( tmpArray[ tmpPos-- ] );
}


template<typename Comparable>
void mergeSort( vector<Comparable> & a, vector<Comparable> & tmpArray,
                int left, int right )
{
    if( left < right ) {
        int center = ( left + right ) / 2;
        mergeSort( a, tmpArray, left, center );
        mergeSort( a, tmpArray, center + 1, right );
        merge( a, tmpArray, left, center + 1, right );
    }
}


template<typename Comparable>
void mergeSort( vector<Comparable> & a )
{
    vector<Comparable> tmpArray( a.size() );

    mergeSort( a, tmpArray, 0, a.size() -1 );
}

int main()
{
    vector<int> vec { 5, 4, 2, 9, 1, 3, 2, 7, 7, 9, 4, 8};
    mergeSort( vec );

    for( auto x : vec )
        cout << x << endl;

    return 0;
}