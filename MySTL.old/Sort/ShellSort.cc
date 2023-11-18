#include <iostream>
#include <vector>

using namespace std;

template<typename Comparable>
void shellSort( vector<Comparable> & a )
{
    for( int gap = a.size() / 2; gap > 0; gap /= 2 ) {
        for( int i = gap; i < a.size(); ++i ) {

            Comparable tmp = std::move( a[ i ] );
            int j = i;

            for( ; j >= gap && tmp < a[ j - gap ]; j -= gap ) {
                a[ j ] = std::move( a[ j - gap ] );
            }
            a[ j ] = std::move( tmp );
        }
    }
}

int main()
{
    vector<int> vec { 5, 4, 2, 9, 1, 3, 7, 8};
    shellSort( vec );
    for( auto x : vec )
        cout << x << endl;

    return 0;
}
