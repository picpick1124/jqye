#include <iostream>
#include <vector>

using namespace std;


void countingRadixSort( vector<string> & arr, int stringLen )
{
    const int BUCKETS = 256;

    int N = arr.size();
    vector<string> buffer( N );

    vector<string> *in = &arr;
    vector<string> *out = &buffer;

    for( int pos = stringLen - 1; pos >= 0; --pos ) {

        vector<int> count( BUCKETS + 1 );

        for( int i = 0; i < N; ++i ) {
            ++count[ (*in)[ i ][ pos ] + 1 ];
        }

        for( int b = 1; b <= BUCKETS; ++b ) {
            count[ b ] += count[ b - 1 ];
        }

        for( int i = 0; i < N; ++i ) {
            ( *out )[ count[ (*in)[ i ][ pos ] ]++ ] = std::move( ( *in )[ i ] );
        }

        std::swap( in, out );
    }

    if( stringLen % 2 == 1 )
        for( int i = 0; i < N; ++i )
            ( *out )[ i ] = std::move( ( *in )[ i ] );
}
int main()
{
    vector<string> s {"qwer", "ddds", "wcoo", "poxn", "oowd" };
    countingRadixSort( s, 4 );
    for( auto x : s )
        cout << x << endl;
    return 0;
}


