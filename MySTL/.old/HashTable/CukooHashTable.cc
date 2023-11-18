#include <algorithm>
#include <string>
#include <vector>
#include "UniformRandom.h"
using namespace std;

int universalHash1( int x, int A, int B, int P, int M )
{
    return static_cast<int> ( ( (static_cast<long long>( A ) * x ) + B ) % P ) % M;
}

int universalHash( int x, int A, int B, int M )
{
    const int DIGS = 31;
    const int mersennep = ( 1 << DIGS ) -1;

    long long hashVal = static_cast<long long>( A ) * x + B;

    hashVal = ( ( hashVal >> DIGS ) + ( hashVal & mersennep ) );
    if( hashVal >= mersennep )
        hashVal -= mersennep;

    return static_cast<int>( hashVal ) % M;
}
template<class AnyType>
class HashFamily{
  public:
    size_t hash( const AnyType & x, int which ) const;
    int getNumberOfFunctions();
    void generateNewFunction();
};

template<int count>
class StringHashFamily {
  public:
    StringHashFamily() : MULTIPLIERS( count )
    {
        generateNewFunctions();
    }

    int getNumberOfFunctions() const
    {
        return count;
    }

    void generateNewFunctions()
    {
        for( auto & mult : MULTIPLIERS )
            mult = r.nextInt();
    }

    size_t hash( const string & x, int which ) const
    {
        const int multiplier = MULTIPLIERS[ which ];
        size_t hashVal = 0;

        for( auto ch : x )
            hashVal = multiplier * hashVal  + ch;

        return hashVal;
    }

  private:
    vector<int> MULTIPLIERS;
    UniformRandom r;
};

template<class AnyType, class HashFamily>
class CukooHashTable{
  public:
    explicit CukooHashTable( int size = 101 )
            : array( nextPrime( size ))
    {
        numHashFunctions = hashFunctions.getNumberOfFunctions();
        rehashes = 0;
        makeEmpty();
    }

    void makeEmpty()
    {
        currentSize = 0;
        for( auto & x : array )
            array.isActive = false;
    }

    bool contains( const AnyType & x ) const
    {
        return findPos( x ) != -1;
    }

    bool remove( const AnyType & x )
    {
        int currentPos = findPos( x );

        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].isActive = false;
        --currentSize;
        return true;
    }

    bool insert( const AnyType & x )
    {
        if( contains( x ) )
            return false;

        if( currentSize >= array.size() * MAX_LOAD )
            expand();

        return insertHelper1( x );
    }

    bool insert( AnyType && x );

private:
    struct HashEntry{
        AnyType element;
        bool isActive;

        HashEntry( const AnyType & e = AnyType{}, bool a = false )
                : element{ e }, isActive{ a } {}

        HashEntry( AnyType&& e, bool a = false )
                : element{ std::move( e ) }, isActive{ a } {}
    };


    bool insertHelper1( const AnyType & x );
    bool insertHelper1( AnyType && xx )
    {
        const int COUNT_LIMIT = 100;
        AnyType x = xx;

        while( true ) {
            int lastPos = -1;
            int pos;

            for( int count = 0; count < COUNT_LIMIT; ++count ) {
                for( int i = 0; i < numHashFunctions; ++i ) {
                    int pos = myhash( x, i );
                    if( !isActive( pos ) ) {
                        array[ pos ] = std::move( HashEntry{ std::move( x ), true } );
                        ++currentSize;
                        return true;
                    }
                }

                int i = 0;
                do {
                    pos = myhash( x, r.nextInt( numHashFunctions ) );
                } while( pos == lastPos && i++ < 5 );

                lastPos = pos;
                std::swap( x, array[ pos ].element );
            }

            if( ++rehashes > ALLOW_REHASHES ) {
                expand();
                rehashes = 0;
            }

            else
                rehash();
        }
    }

    bool isActive( int currentPos ) const
    {
        return array[ currentPos ].isActive == true;
    }

    size_t myhash( const AnyType & x, int which ) const
    {
        return hashFunctions.hash( x, which ) % array.size();
    }

    int findPos( const AnyType & x ) const
    {
        for( int i = 0; i < numHashFunctions; ++i ) {
            int pos = myhash( x, i );

            if( isActive( pos ) && array[ pos ].element == x )
                return pos;
        }

        return -1;
    }

    void expand()
    {
        rehash( static_cast<int>( array.size() / MAX_LOAD) );
    }

    void rehash()
    {
        hashFunctions.generateNewFunctions();
        rehash( array.size() );
    }

    void rehash( int newSize )
    {
        vector<HashEntry> oldArray = array;

        array.resize( nextPrime( newSize ) );

        for( auto & entry : array ) {
            entry.isActive = false;
        }

        currentSize = 0;
        for( auto & entry : oldArray ) {
            if( entry.isActive )
                insert( std::move( entry.element ) );
        }
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

    static constexpr double MAX_LOAD       = 0.40;
    static constexpr int    ALLOW_REHASHES = 5;

    vector<HashEntry> array;
    int currentSize;
    int numHashFunctions;
    int rehashes;
    UniformRandom r;
    HashFamily hashFunctions;
};

int main()
{
    return 0;
}
