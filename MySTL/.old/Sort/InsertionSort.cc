#include <iostream>
#include <vector>

using namespace std;

template<typename Comparable>
void insertionSort( vector<Comparable> & a)
{
    for( int p = 1; p < a.size(); ++p ) {
        Comparable tmp = std::move( a[ p ] );

        int j;
        for( j = p; j > 0 && tmp < a[ j - 1 ]; --j ) {
            a[ j ] = std::move( a[ j - 1 ] );
        }

        a[ j ] = std::move( tmp );
    }
}

template<typename Iterator, typename Comparotor>
void insertionSort( const Iterator & begin, const Iterator & end,
                    Comparotor lessThan )
{
    if( begin == end )
        return;

    Iterator j;

    for( Iterator p = begin + 1; p != end; ++p ) {
        auto tmp = std::move( *p );

        for( j = p; j != begin && lessThan( tmp, *(j - 1) ); --j ) {
            *j = std::move( *(j -1) );
        }

        *j = std::move( tmp );
    }
}


template<typename Iterator>
void insertionSort( const Iterator & begin, const Iterator & end )
{
    insertionSort( begin, end, less<decltype(*begin)>{} );
}

int main()
{
    vector<int> vec { 5, 4, 2, 9, 1, 3, 7, 8};
    vector<int>::iterator a = vec.begin();
    vector<int>::iterator b = vec.end();
    insertionSort( a, b );
    for( auto x : vec )
        cout << x << endl;

    return 0;
}
