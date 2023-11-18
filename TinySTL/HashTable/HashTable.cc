#include <iostream>
#include <fstream>
#include <list>
#include <vector>

using namespace std;

template<class key>
class hashd{
  public:
    size_t operator() ( const key & k ) const ;
};

template <>  //模板特化，必须有一个基础的函数模板,这里使用这名字是因为会有冲突
class hashd<string> {
  public:
    size_t operator()( const string & key )
    {
        size_t hashVal = 0;

        for( char ch : key )
            hashVal = 37 * hashVal + ch;

        return hashVal;
    }
};

template<class HashedObj>
class HashTable{
  public:
    explicit HashTable( int size =  101 );

    bool contains( const HashedObj & x ) const
    {
        auto & whichList = theLists[ myhash(x) ];
        return find( begin( whichList ), end( whichList ), x ) != end( whichList );
    }

    void makeEmpty()
    {
        for( auto & thisList : theLists )
            thisList.clear();
    }

    bool insert( const HashedObj & x )
    {
        auto & whichList = theLists[ myhash( x )];
        if( find( begin( whichList ), end( whichList ), x) != end( whichList ) )
            return false;

        whichList.push_back( x );

        if( ++currentSize > theLists.size() )
            rehash();

        return true;
    }
    bool insert( HashedObj && x );
    bool remove( const HashedObj & x )
    {
        auto & whichList = theLists[ myhash( x ) ];
        auto itr = find( begin( whichList ), end( whichList ), x);

        if( itr == end( whichList ) )
            return false;

        whichList.erase( itr );
        --currentSize;
        return true;
    }


  private:
    vector<list<HashedObj>> theLists;
    int currentSize;

    void rehash()
    {
        vector<list<HashedObj>> oldLists = theLists;

        theLists.resize( nextPrime( 2 * oldLists.size() ) );
        for( auto & thisList : oldLists )
            thisList.clear();

        currentSize = 0;
        for( auto & thisList : oldLists )
            for( auto & x : thisList )
                insert( std::move( x ) );
    }

    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % theLists.size();
    }
};

class Employee {
  public:
    const string & get_name() const
    {
        return name;
    }

    bool operator==( const Employee & rhs )
    {
        return ( get_name() == rhs.get_name() );
    }

    bool operator!=( const Employee & rhs )
    {
        return !( *this == rhs );
    }
  private:
    string name;
    double salary;
    int seniority;
};

template <>
class hashd<Employee>
{
  public:
    size_t operator()( const Employee & item )
    {
        static hashd<string> hf;
        return hf( item.get_name() );
    }
};
int main()
{
    HashTable<int> a;
    return 0;
}
