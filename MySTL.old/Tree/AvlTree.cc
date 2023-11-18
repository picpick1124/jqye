#include <iostream>
#include <initializer_list>
#include <vector>
#include <algorithm>

using namespace std;

template<class Comparable>
class BinarySearchTree{
public:
    BinarySearchTree( ) : root{ nullptr } {}
    BinarySearchTree( const BinarySearchTree & rhs ) : root{ nullptr }
    {
        root = clone( rhs.root );
    }
    BinarySearchTree( BinarySearchTree && rhs ) : root( rhs.root )
    {
        rhs.root = nullptr;
    }
    ~BinarySearchTree( ) { makeEmpty(); }

    const Comparable & findMax( ) const
    {
        return findMax( root )->element;
    }
    const Comparable & findMin( ) const
    {
        return findMin( root )->element;
    }

    bool contains( const Comparable & x ) const
    {
        return contains( x, root );
    }

    bool isEmpty( ) const
    {
        return root == nullptr;
    }

    void printTree( ) const
    {
        if( isEmpty( ) )
            cout << "Empty tree" << endl;
        else
            printTree( root, cout );
    }

    void makeEmpty( )
    {
        makeEmpty( root );
    }
    void insert( const Comparable & x )
    {
        insert( x, root );
    }

    void insert( Comparable && x )
    {
        insert( x, root );
    }

    void remove( const Comparable & x )
    {
        remove( x, root );
    }


    BinarySearchTree & operator=( const BinarySearchTree& rhs )
    {
        BinarySearchTree copy = rhs;
        std::swap( *this, copy );
        return *this;
    }

    BinarySearchTree operator=( BinarySearchTree && rhs )
    {
        std::swap( root, rhs.root );
        return *this;
    }

private:
    struct AvlNode{
        Comparable element;
        AvlNode *left;
        AvlNode *right;
        int     height;

        AvlNode( const Comparable & ele, AvlNode *lt, AvlNode *rt, int h = 0  )
                :element{ ele }, left{ lt }, right{ rt }, height{ h } { }
        AvlNode( Comparable&& ele, AvlNode *lt, AvlNode *rt, int h = 0 )
                :element{ std::move( ele ) }, left{ lt }, right{ rt }, height{ h } { }
     };

    AvlNode *root;

    int height( AvlNode* t ) const
    {
        return ( t == nullptr ) ? -1 : t->height;
    }

    static const int ALLOWED_IMBALANCE = 1;

    void balance( AvlNode * & t)
    {
        if( t == nullptr )
            return;
        if( height( t->left ) - height( t->right ) > ALLOWED_IMBALANCE ) {
            if( height( t->left->left ) >= height( t->left->right ) ) //等号对应的是删除的情况，保证了在删除情况下使用单旋转
                rotateWithLeftChild( t );
            else
                doubleWithLeftChild( t );
        }
        if( height( t->right ) - height( t->left ) > ALLOWED_IMBALANCE ) {
            if( height( t->right->right ) >= height( t->right->left ) ) //等号对应的是删除的情况，保证了在删除情况下使用单旋转
                rotateWithRightChild( t );
            else
                doubleWithRightChild( t );
        }
        t->height = max( height( t->left ), height( t-> right ) ) + 1;
    }

    void rotateWithLeftChild( AvlNode * & k2 )
    {
        AvlNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
        k1->height = max( height( k1->left ), k2->height ) + 1;
        k2 = k1;
    }

    void rotateWithRightChild( AvlNode * & k2 )
    {
        AvlNode *k1 = k2->right;
        k2->right = k1->left;
        k1->left = k2;
        k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
        k1->height = max( height( k1->right ), k2->height ) + 1;
        k2 = k1;
    }

    void doubleWithLeftChild( AvlNode * & k3 )
    {
        rotateWithRightChild( k3->left );
        rotateWithLeftChild( k3 );
    }

    void doubleWithRightChild( AvlNode * & k1 )
    {
        rotateWithLeftChild( k1->right );
        rotateWithRightChild( k1 );
    }

    void insert( const Comparable & x, AvlNode * & t )
    {
        if( t == nullptr )
            t = new AvlNode{ x, nullptr, nullptr };
        if( x < t->element )
            insert( x, t->left );
        else if( t->element < x )
            insert( x, t->right );
        balance( t );
    }

    void insert( Comparable&& x, AvlNode * & t )
    {
        if( t == nullptr )
            t = new AvlNode{ std::move(x), nullptr, nullptr };
        if( x < t->element )
            insert( std::move(x), t->left );
        else if( t->element < x )
            insert( std::move(x), t->right );
        balance( t );
    }


    void remove( const Comparable & x, AvlNode * & t )
    {
        if( t == nullptr )
            return;
        if( x < t->element )
            remove( x, t->left );
        else if( t->element < x )
            remove( x, t->right );
        else if( t->left != nullptr && t->right != nullptr ) {
            t->element = findMin( t->right )->element;
            remove( t->element,  t->right );
        }
        else {
            AvlNode *oldNode = t;
            t = ( t->left != nullptr ) ? t->left : t->right;
            delete oldNode;
        }

        balance( t );
    }

    AvlNode * findMax( AvlNode *t ) const
    {
        if( t != nullptr )
            while( t-> right != nullptr )
                t = t->right;
        return t;
    }

    AvlNode * findMin( AvlNode *t ) const
    {
        if ( t == nullptr )
            return nullptr;
        if( t->left == nullptr )
            return t;
        return findMin( t-> left );
    }

    bool contains( const Comparable & x, AvlNode *t ) const
    {
        if( t == nullptr )
            return false;
        else if( x < t->element )
            return contains( x, t->left );
        else if( t->element < x )
            return contains( x, t->right );
        else
            return true;

        // if( x == t -> element ) { return true; }

        // if( x < t -> element ) {
        //     contains( x, t -> left );
        // }
        // else {
        //     contains( x, t -> right );
        // }

    }
    void makeEmpty( AvlNode * & t )
    {
        if( t !=nullptr ) {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = nullptr;
    }

    void printTree( AvlNode *t, ostream & out) const
    {
        if( t != nullptr ) {
            printTree( t->left, out );
            out << t->element << endl;
            printTree( t->right, out );
        }
    }
    AvlNode * clone( AvlNode *t ) const
    {
        if( t == nullptr )
            return nullptr;
        else
            return new AvlNode{ t->element, clone( t->left ), clone( t->right )};
    }

 };

int main()
{
    BinarySearchTree<int> byt;
    byt.insert(8);
    byt.insert(2);
    byt.insert(3);
    byt.insert(41);
    byt.insert(15);
    byt.printTree();
    cout << "--------" << endl;
    byt.remove(3);
    byt.printTree();
    cout << "--------" << endl;
    const int max = byt.findMax();
    const int min = byt.findMin();

    cout << min << endl;
    cout << max << endl;
    return 0;
 }
