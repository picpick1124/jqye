#include <iostream>
#include <vector>

using namespace std;

void uradixSort( vector<string> & arr, int maxLen )
{
    const int BUCKETS = 256;

    vector<vector<string>> wordsByLength( maxLen + 1 );
    vector<vector<string>> buckets( BUCKETS );

    for( string & s : arr )
        wordsByLength[ s.length() ].push_back( std::move( s ) );

    int idx = 0;
    for( auto & wordList : wordsByLength )
        for( string & s : wordList )
            arr[ idx++ ] = std::move( s );


    int startingIndex = arr.size();
    for( int pos = maxLen - 1; pos >= 0; --pos ) {
        startingIndex -= wordsByLength[ pos + 1 ].size();

        for( int i = startingIndex; i < arr.size(); ++i )
            buckets[ arr[ i ][ pos ] ].push_back( std::move( arr[ i ] ) );

        idx = startingIndex;
        for( auto & thisBucket: buckets ) {
            for( string & s : thisBucket )
                arr[ idx++ ] = std::move( s );

            thisBucket.clear();
        }
    }
}

int main()
{
    vector<string> s {"qwer", "d", "ddds", "wcoo", "poxn", "oowd" };
    uradixSort( s, 4 );
    for( auto x : s )
        cout << x << endl;
    return 0;
}


