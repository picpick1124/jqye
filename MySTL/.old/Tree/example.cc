#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

void printHighChangeables( const map<string,vector<string>> & adjacentWords,
                           int minWords = 15 )
{
    for( auto & entry : adjacentWords ) {
        const vector<string> & words = entry.second;

        if( words.size() >= minWords ) {
            cout << entry.first << "(" <<  words.size() << "):";
            for( auto & str : words ) {
                cout << " " << str;
            }
            cout << endl;
        }
    }
}

bool oneCharOff( const string & word1, const string & word2 )
{
    if( word1.length() != word1.length() )
        return false;

    int diffs = 0;

    for( int i = 0; i < word1.length(); ++i ) {
        if( word1[i] != word2[i] ) {
            if( ++diffs > 1 )
                return false;
        }
    }
    return diffs == 1;
}

map<string, vector<string>>
computeAdjacentWords( const vector<string> & words )
{
    map<string,vector<string>> adjWords;
    map<int,vector<string>> wordsByLength;

    for( auto & str : words )
        wordsByLength[ str.length() ].push_back( str );

    for( auto & entry : wordsByLength ) {
        const vector<string> & groupWords = entry.second;
        int groupNum = entry.first;

        for( int i = 0; i < groupNum; ++i ) {

            map<string,vector<string>> repToWord;

            for( auto & str : groupWords ) {
                string rep = str;
                rep.erase( i, 1 );
                repToWord[ rep ].push_back( str );
            }

            for( auto & entry : repToWord )
            {
                const vector<string> & clique = entry.second;
                if( clique.size() >= 2 )
                {
                    for( int p = 0; i < clique.size(); ++i )
                        for( int q = p + 1; i < clique.size(); ++i )
                        {
                            adjWords[ clique[ p ] ].push_back( clique[ q ] );
                            adjWords[ clique[ q ] ].push_back( clique[ p ] );
                        }
                }
            }

        }
    }

    return adjWords;
}

void is( vector<string>& vec_str )
{
    string name;
    cin >> name;
    ifstream ist( name.c_str() );
    string str;
    while( ist >> str )
        vec_str.push_back( str );
}

int main()
{
    vector<string> vec_str;
    is( vec_str );
    map<string,vector<string>> wordMap = computeAdjacentWords( vec_str );
    printHighChangeables( wordMap );
    return 0;
}
