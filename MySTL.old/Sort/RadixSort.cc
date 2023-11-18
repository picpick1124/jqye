#include <iostream>
#include <vector>

using namespace std;

void radixSort( vector<string> & arr, int stringLenth )
{

    const int BUCKETS = 256;
    vector<vector<string>> buckets( BUCKETS );

    for( int pos = stringLenth - 1; pos >= 0; --pos ) {

        for( string & s : arr ) {
            buckets[ s[ pos ] ].push_back( std::move( std::move( s ) ) );
        }

        int idx = 0;
        for( auto & thisBucket : buckets ) {
            for( string & s : thisBucket )
                arr[ idx++ ] = std::move( s );

            thisBucket.clear();
        }
    }

}

int main()
{
    vector<string> s {"qwer", "ddds", "wcoo", "poxn", "oowd" };
    radixSort( s, 4 );
    for( auto x : s )
        cout << x << endl;
    return 0;
}


