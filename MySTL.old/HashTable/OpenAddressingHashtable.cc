#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

template<class key>
class hashd{
  public:
    size_t operator()( const key & k ) const;
};

template<>
class hashd<string> {
    size_t operator()( const string & key )
    {
        size_t hashValue = 0;

        for( char ch : key )
            // hashValue = ( hashValue + 37*ch ) * 37;
            hashValue = 37 * hashValue + ch; //37是x, ch 是系数

        return hashValue;
    }
};



template<class HashObj>
class HashTable {
  public:
    explicit HashTable( int size = 101 )
            : array( nextPrime( size ) ) { makeEmpty(); }

    bool contains( const HashObj & x ) const
    { return isActive( findPos( x ) ); }

    void makeEmpty()
    {
        currentSize = 0;
        for( auto & entry : array )
            entry.info = EMPTY;
    }

    bool insert( const HashObj & x )
    {
        int currentPos = findPos( x );

        if( isActive( x ) )
            return false;

        array[ currentPos ].element = x;
        array[ currentPos ].info = ACTIVE;

        if( ++currentPos > currentSize/2 )
            rehash();

        return true;
    }

    bool insert( HashObj && x )
    {
        int currentPos = findPos( std::move( x ) );

        if( isActive( std::move( x ) ) )
            return false;

        array[ currentPos ].element = std::move( x );
        array[ currentPos ].info = ACTIVE;

        if( ++currentPos > currentSize/2 )
            rehash();

        return true;
    }

    bool remove( const HashObj & x )
    {
        int currentPos = findPos( x );

        if( ! isActive( currentPos ) )
            return false;

        array[ currentPos ].info = DELETED;

        return true;
    }

    enum EntryType { ACTIVE, EMPTY, DELETED };

  private:
    struct HashEntry{
        HashObj element;
        EntryType info;

        HashEntry( const HashObj & e = HashObj{}, EntryType i = EMPTY )
                : element{ e }, info{ i } { }
        HashEntry( HashObj && e, EntryType i = EMPTY )
                : element{ std::move( e ) }, info{ i } { }

    };

    vector<HashEntry> array;
    int currentSize;

    bool isActive( int currentPos ) const
    {
        return array[ currentPos ].info == ACTIVE;//
    }
    int findPos( const HashObj & x ) const
    {
        int offset = 1;
        int currentPos = myhash( x );

        while( array[ currentPos ].info != EMPTY && array[ currentPos ].element != x ) {
            currentPos += offset;
            offset += 2;

            if( currentPos >= array.size() )
                currentPos -= array.size();
        }

        return currentPos;
    }
    void rehash()
    {
        vector<HashEntry> oldArray = array;

        array.resize( nextPrime( 2 * oldArray.size( ) ) );

        for( auto & entry : array )
            entry.info = EMPTY;

        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.info == ACTIVE)
                insert( std::move( entry.element ) );
    }
    size_t myhash( const HashObj & x ) const
    {
        static hashd<HashObj> hf;
        return hf( x ) % currentSize;
    }


    int nextPrime( int n )
    {
        if( n % 2 == 0 )
            ++n;

        for( ; !isPrime( n ); n += 2 )
            ;

        return n;
    }

    bool isPrime( int n )
    {
        if( n == 2 || n == 3 )
            return true;

        if( n == 1 || n % 2 == 0 )
            return false;

        for( int i = 3; i * i <= n; i += 2 )
            if( n % i == 0 )
                return false;

        return true;
    }
};

int main()
{
    vector<int> vec{3,2,43, 22242134,3, 403082,3223};
    vector<int> vec1{3,2,43, 22242134,3, 403082,3223};
    HashTable<int> hashtable;

    for( auto& element : vec)
        if(!hashtable.insert(element)){
            cout << "insert is faild" << endl;
            break;
        }


    for( auto& element : vec1 )
        hashtable.remove(element);


    return 0;
}
