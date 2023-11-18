#include <iostream>

using namespace std;

template<class Comparable>
class LeftistHeap {
  public:
    LeftistHeap( );
    LeftistHeap( const LeftistHeap & rhs );
    LeftistHeap( LeftistHeap && rhs );

    ~LeftistHeap();

    LeftistHeap & operator=( const LeftistHeap & rhs );
    LeftistHeap & operator=( LeftistHeap && rhs );


    bool isEmpty() const
    {
        return root == nullptr;
    }
    const Comparable & findMin() const;

    void insert( const Comparable & x )
    {
        root = merge( new LeftistNode{ x }, root );
    }

    void insert( Comparable && x );
    void deleteMin()
    {
        if( isEmpty() )
            return;
        LeftistNode *oldRoot = root;
        root = merge( root->left, root->right );
        delete oldRoot;
    }

    void deleteMin( Comparable & minItem )
    {
        minItem = findMin();
        deleteMin();
    }

    void makeEmpty()
    {
        while( !isEmpty() )
            deleteMin();
    }
    void merge( LeftistHeap & rhs )
    {
        if( this == &rhs )
            return;

        root = merge( root, rhs.root );
        rhs.root = nullptr;
    }

  private:
    struct LeftistNode {
        Comparable   element;
        LeftistNode *left;
        LeftistNode *right;
        int          npl;

        LeftistNode( const Comparable & e = Comparable{ }, LeftistNode *lt = nullptr,
                     LeftistNode *rt = nullptr, int np = 0)
                : element{ e }, left{ lt }, right{ rt }, npl{ np } {}

        LeftistNode( Comparable && e = Comparable{ }, LeftistNode *lt = nullptr,
                     LeftistNode *rt = nullptr, int np = 0)
                : element{ std::move( e ) }, left{ lt }, right{ rt }, npl{ np } {}

    };

    LeftistNode *root;

    LeftistNode * merge( LeftistNode *h1, LeftistNode *h2 )
    {
        if( h1 == nullptr )
            return h2;
        if( h2 == nullptr )
            return h1;

        if( h1->element < h2->element )
            return merge1( h1, h2 );
        else
            return merge1( h2, h1 );
    }

    LeftistNode * merge1( LeftistNode *h1, LeftistNode *h2 )
    {
        if( h1->left == nullptr )
            h1->left = h2;

        else {
            h1->right = merge( h1->right, h2 );
            if( h1->left->npl < h1->right->npl )
                swapChildren( h1 );
            h1->npl = h1->right->npl + 1;
        }

        return h1;
    }

    void swapChildren( LeftistNode *t );
    void reclaimMemory( LeftistNode *t );
    LeftistNode * clone( LeftistNode * t ) const;
};


int main()
{
    return 0;
}