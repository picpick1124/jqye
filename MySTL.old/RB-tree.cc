#include <iostream>
class UnderflowException { };

using namespace std;
template < class Comparable >
class redblack_tree
{
  private:
    struct redblack_node
    {
        Comparable element;
        redblack_node *left;
        redblack_node *right;
        int color;

        redblack_node( const Comparable & the_element = Comparable{},
                       redblack_node *lt = nullptr, redblack_node *rt = nullptr,
                       int c = BLACK )
                : element{ the_element }, left{ lt }, right{ rt }, color{ c } {}
        redblack_node( Comparable && the_element, redblack_node *lt = nullptr,
                       redblack_node *rt = nullptr, int c = BLACK )
                : element{ std::move( the_element )}, left{ lt }, right{ rt }, color{ c }{}
    };

    redblack_node *header;
    redblack_node *null_node;

    redblack_node *current;
    redblack_node *parent;
    redblack_node *grand;
    redblack_node *great;

    void reclaim_memory( redblack_node *t );

    void print_tree( redblack_node *t ) const
    {
        if( t != t -> left ) {
            print_tree( t -> left );
            cout << t -> element << endl;
            print_tree( t -> right );
        }
    }


    redblack_node *clone ( redblack_node *t ) const
    {
        if( t == t -> left )
            return null_node;
        else
            return new redblack_node{ t -> element, clone( t -> left ),
                clone( t -> right ), t -> color };
    }

    void handle_reorient( const Comparable & item )
    {
        current -> color = RED;
        current -> left -> color = BLACK;
        current -> right -> color = BLACK;

        if( parent -> color == RED ) {
            grand -> color = RED;
            if( item < grand -> element != item < parent -> element )
                parent = rotate( item, grand );
            current = rotate( item, great );
            current -> color = BLACK;
        }

        header -> right -> color = BLACK;
    }



    redblack_node * rotate( const Comparable & item, redblack_node * the_parent )
    {
        if( item < the_parent -> element ) {
            item < the_parent -> left -> element ?
                    rotate_with_left_child( the_parent -> left ) :
                    rotate_with_right_child( the_parent -> left ) ;

            return the_parent -> left;
        } else {
            item < the_parent -> right -> element ?
                    rotate_with_left_child( the_parent -> right ) :
                    rotate_with_right_child( the_parent -> right ) ;

            return the_parent -> right;
        }
    }

    void rotate_with_left_child( redblack_node * & k2 )
    {
        redblack_node* k1 = k2 -> left;
        k2 -> left = k1 -> right;
        if(k1 -> right)
            k1 -> right -> parent = k2;

        k1 -> parent = k2 -> parent;
        if(nullptr == k2 -> parent)
            header = k1;
        else if(k2 -> parent -> left == k2)
            k2 -> parent -> left = k1;
        else
            k2 -> parent -> kright = k1;

        k1 -> right = k2;
        k2 -> parent = k1;
    }

    void rotate_with_right_child( redblack_node * & k1 )
    {
        redblack_node* k2 = k1 -> right;
        k1 -> right = k2 -> left;
        if(k2 -> left)
            k2 -> left -> parent = k1;

        k2 -> parent = k1 -> parent;
        if( nullptr == k1 -> parent )
            header = k2;
        else if(k1 -> parent -> left == k1)
            k1 -> parent -> left = k2;
        else
            k1 -> parent -> right = k2;

        k2 -> left = k1;
        k1 -> parent = k2;
    }



  public:
    explicit redblack_tree( const Comparable & neg_inf )
    {
        null_node = new redblack_node;
        null_node -> left = null_node -> right = null_node;

        header = new redblack_node{ neg_inf };
        header -> left = header -> right = null_node;
    }


    redblack_tree( const redblack_tree & rhs )
    {
        null_node = new redblack_node;
        null_node -> left = null_node -> right = null_node;

        header = new redblack_node{ rhs.header -> element };
        header -> left = null_node;
        header -> right = clone( rhs.header -> right );
    }

    redblack_tree( redblack_tree && rhs )
    : header{rhs.header}
    {
        rhs.header = nullptr;
    }

    ~redblack_tree()
    {
        make_empty();
        header = nullptr;
    }


    const Comparable & find_min() const
    {
        if( is_empty() )
            throw UnderflowException{};
        redblack_node* ptr = header;
        while( ptr -> left )
            ptr = ptr -> left;
        return ptr -> element;
    }

    const Comparable & find_max() const
    {
        if( is_empty() )
            throw UnderflowException{};
        redblack_node* ptr = header;
        while( ptr -> right )
            ptr = ptr -> right;
        return ptr -> element;
    }


    bool contains( const Comparable & x ) const
    {
        redblack_node* ptr = header;
        while( ptr )
        {
            if( x < ptr-> element)
                ptr = ptr -> left;
            else if( ptr -> element < x)
                ptr = ptr -> right;
            else
                return true;
        }
        return false;
    }

    bool is_empty() const
    {
        return header == nullptr;
    }

    void print_tree( ) const
    {
        if( header -> right == null_node )
            cout << "Empty tree" << endl;
        else
            print_tree( header -> right );
    }

    void make_empty()
    {
        if( nullptr == header )
            return;
        redblack_node *lt = header -> left;
        redblack_node *rt = header -> right;
        delete header;
        make_empty( lt );
        make_empty( rt );
    }

    void insert( const Comparable & x )
    {
        current = parent = grand = header;
        null_node -> element = x;

        while( current -> element !=x ) {
            great = grand; grand = parent; parent = current;
            current = x < current -> element ? current -> left : current -> right;
            if( current -> left -> color == RED && current -> right -> color == RED )
                handle_reorient( x );
        }

        if( current != null_node )
            return;

        current = new redblack_node{ x, null_node, null_node };

        if( x < parent -> element )
            parent -> left = current;
        else
            handle_reorient( x );
    }

    void remove( const Comparable & x )
    {
        if( is_empty() )
            return;
        remove( x, header );
    }

    enum{ RED, BLACK };

    redblack_tree & operator=( const redblack_tree & rhs )
    {
        redblack_tree copy{rhs};
        std::swap(*this, copy);
        return *this;
    }

    redblack_tree & operator=( redblack_tree && rhs )
    {
        std::swap( header, rhs. header);
        return *this;
    }


};

//检测
int main()
{
    return 0;
}
