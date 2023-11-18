#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>
#include <type_traits>

using namespace std;

// 必须有这个
template<class key>
class hashd{
  public:
    size_t operator() ( const key & k ) const ;
};

//模板特化，必须有一个基础的函数模板,这里使用这名字是因为会有冲突
// 哈希函数
template <>
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

template<class Node>


template<class HashedObj>
class HashTable{
  public:
    explicit HashTable( int size =  101 ):theLists(size){}

    size_t size() const
    {
        return currentSize;
    }

    // 这个好像可以复用, 还不错
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

    template<typename InsertObj>
    bool insert( InsertObj && x )
    {
        // 不要判断插入元素的类型
        using U1 = typename std::remove_reference<InsertObj>::type;
        static_assert(is_same<U1, HashedObj>::value == true,
                      "HashTable element type is different from HashTable");

        auto & whichList = theLists[ myhash( forward<InsertObj>(x) )];
        if( find( begin( whichList ), end( whichList ), forward<InsertObj>( x )) != end( whichList ) )
            return false;

        whichList.push_back( forward<InsertObj>( x ) );

        if( ++currentSize > theLists.size() )
            rehash();

        return true;
    }

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

// 重载 mystl 的 swap
template <class T, class Hash, class KeyEqual>
void swap(HashTable<T, Hash, KeyEqual>& lhs,
          hashtable<T, Hash, KeyEqual>& rhs) noexcept
{
  lhs.swap(rhs);
}
