#include <iostream>
#include <vector>

using namespace std;

template<typename Comparable>
void quicksort( vector<Comparable> & items )
{
    if( items.size() > 1 ) {

        vector<Comparable> smaller;
        vector<Comparable> same;
        vector<Comparable> larger;

        auto chosenItem = items[ items.size() / 2 ];

        for( auto & x : items ) {
            if( x < chosenItem )
                smaller.push_back( std::move( x ) );
            else if( x == chosenItem )
                same.push_back( std::move( x ) );
            else
                larger.push_back( std::move( x ) );
        }

        quicksort( smaller );
        quicksort( larger );

        std::move( begin( smaller ), end( smaller ), begin( items ) );
        std::move( begin( same ), end( same ), begin( items ) + smaller.size() );
        std::move( begin( larger ), end( larger ), end( items ) - larger.size() );
    }
}
int main()
{
    vector<int> vec { 5, 4, 2, 9, 1, 3, 7, 9, 4, 8};
    quicksort( vec );

    for( auto x : vec )
        cout << x << endl;

    return 0;
}