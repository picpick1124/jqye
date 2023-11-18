#include <iostream>
#include <vector>

using namespace std;

template<class Comparable>
class BinomialQueue{
  public:
    BinomialQueue();
    BinomialQueue( const Comparable & item );
    BinomialQueue( const BinomialQueue & rhs );
    BinomialQueue( BinomialQueue && rhs );

    ~BinomialQueue();

    BinomialQueue & operator=( const BinomialQueue & rhs );
    BinomialQueue & operator=( BinomialQueue && rhs );

    bool isEmpty() const;
    const Comparable & findMin() const;

    void insert( const Comparable & x );
    void insert( Comparable && x );

    void deleteMind( Comparable & minItem )
    {
        if( isEmpty() )
            return;

        int minIndex = findMinIndex();
        minItem = theTrees[ minIndex ]->element;

        BinomialNode *oldRoot = theTrees[ minIndex ];
        BinomialNode *deletedTree = oldRoot->leftChild;
        delete oldRoot;

        //H''
        BinomialQueue deletedQueue;
        deletedQueue.theTrees.resize( minIndex + 1);
        deletedQueue.currentSize = ( 1 << minIndex ) -1;
        for( int j = minIndex -1; j >= 0; --j ) {
            deletedQueue.theTrees[ j ] = deletedTree;
            deletedTree = deletedTree -> nextSibling;
            deletedQueue.theTrees[ j ]->nextSibling = nullptr;
        }

        //H'
        theTrees[ minIndex ] = nullptr;
        currentSize -= deletedQueue.currentSize + 1;

        merge( deletedQueue );

    }

    void makeEmpty();
    void merge( BinomialQueue & rhs )
    {
        if( this == &rhs )
            return;

        currentSize += rhs.size();

        if( currentSize > theTrees.size() ) {
            int oldNumtrees = theTrees.size();
            int newNumTrees = max( theTrees.size(), rhs.theTrees.size() ) + 1;
            theTrees.resize( newNumTrees );
            for( int i = oldNumtrees; i < newNumTrees; ++i )
                theTrees[ i ] = nullptr;
        }

        BinomialNode *carry = nullptr;
        for( int i = 0, j = 1; j <=currentSize; ++i, j *= 2) {
            BinomialQueue *t1 = theTrees[ i ];
            BinomialQueue *t2 = i < rhs.theTrees.size() ? rhs.theTrees[ i ]
                                                        : nullptr;

            int whichCase  = t1 == nullptr ? 0 : 1;
            whichCase     += t2 == nullptr ? 0 : 2;
            whichCase     += carry == nullptr ? 0 : 4;

            switch( whichCase ) {
              case 0:
              case 1:
                break;
              case 2:
                theTrees[ i ] = t2;
                rhs.theTrees[ i ] = nullptr;
                break;
              case 4:
                theTrees[ i ] = carry;
                carry = nullptr;
                break;
              case 3:
                carry = combineTrees( t1, t2 );
                theTrees[ i ] = rhs.theTrees[ i ] = nullptr;
                break;
              case 5:
                carry = combineTrees( t1, carry );
                theTrees[ i ] = nullptr;
                break;
              case 6:
                carry = combineTrees( t2, carry );
                rhs.theTrees[ i ] = nullptr;
                break;
              case 7:
                theTrees[ i ] = carry;
                carry = combineTrees( t1, t2 );
                rhs.theTrees[ i ] = nullptr;
                break;
            }
        }

        for( auto & root : rhs.theTrees )
          root = nullptr;

        rhs.currentSize = 0;
    }

  private:
    struct BinomialNode{
        Comparable    element;
        BinomialNode *leftChild;
        BinomialNode *nextSibling;

        BinomialNode( const Comparable & e, BinomialNode *lt, BinomialNode *rt )
                : element{ e }, leftChild{ lt }, nextSibling{ rt } {}

        BinomialNode( Comparable && e, BinomialNode *lt, BinomialNode *rt )
                : element{ std::move( e ) }, leftChild{ lt }, nextSibling{ rt } {}
    };

    const static int DEFAULT_TREES = 1;

    vector<BinomialNode *> theTrees;
    int currentSize;

    int findMinIndex() const
  {
    int i;
    int minIndex;

    for( i = 0; theTrees[ i ] == nullptr; ++i )
      ;

    for( minIndex = i; i < theTrees.size(); ++i )
      if( theTrees[ i ] != nullptr &&
          theTrees[ i ]->element < theTrees[ minIndex ]->element )
        minIndex = i;

    return minIndex;
  }

    int capacity() const
    {
        return theTrees.size();
    }

    BinomialNode * combineTrees( BinomialNode *t1, BinomialNode *t2 )
    {
        if( t2->element < t1->element )
            return combineTrees( t2, t1 );

        t2->nextSibling = t1->leftChild;
        t1->leftChild = t2;
        return t1;
    }

    void makeEmpty( BinomialNode * & t);
    BinomialNode * clone( BinomialNode * t ) const;
};


int main()
{
    return 0;
}
