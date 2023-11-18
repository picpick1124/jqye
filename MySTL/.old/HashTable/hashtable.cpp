#include <cstddef>
#include <iostream>
// #include <iterator>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility> // 包含 std::pair
#include <memory.h>

// 使用SFINAE技术来检查传入的类型是否为std::pair
template <class T, T v>
struct m_integral_constant
{
  static constexpr T value = v;
};

template <bool b>
using m_bool_constant = m_integral_constant<bool, b>;

typedef m_bool_constant<true>  m_true_type;
typedef m_bool_constant<false> m_false_type;

template<class T>
struct is_pair final : m_false_type {};

template<class T1,class T2>
struct is_pair<std::pair<T1, T2>> final : m_true_type{} ;

template<class T>
struct hashtableNode;

template <class T, class hashFun, class keyEqual>
class hashtable;

template <class T, class hashFun, class keyEqual>
struct iterator_base;

template <class T, class hashFun, class keyEqual>
struct iterator;

template <class T, class hashFun, class keyEqual>
struct const_iterator;

template <class T>
struct local_iterator;

template <class T>
struct const_local_iterator;


// 获取pair的first成员
template<class T, bool>
struct value_traits_imp {
    using key_type    = T;
    using mapped_type = T;
    using value_type  = T;

    template<class T1>
    static const key_type& get_key( const T1& value )
    {
        return value;
    }

    template<class T1>
    static const key_type& get_value( const T1& value )
    {
        return value;
    }
};

template <class T>
struct value_traits_imp<T, true>{
    // 移除 const volatile
    using key_type    = typename std::remove_cv<typename T::first_type>::type;
    using mapped_type = typename T::second_type;
    using value_type  = T;

    template<class T1>
    static const key_type& get_key( const T1& value)
    {
        return value.first;
    }

    template<class T1>
    static const value_type& get_value( const T1& value )
    {
        return value;
    }

};

template <class T>
struct value_traits{
    static constexpr bool is_map = is_pair<T>::value;

    using value_traits_type = value_traits_imp<T, is_map>;

    using key_type    = typename value_traits_type::key_type;
    using mapped_type = typename value_traits_type::mapped_type;
    using value_type  = typename value_traits_type::value_type;

    template<class T1>
    static const key_type& get_key(const T1& value)
    {
        return value_traits_type::get_key(value);
    }

    template<class T1>
    static const key_type& get_value(const T1& value)
    {
        return value_traits_type::get_key(value);
    }

};


// 分离链接法的节点
template<class T>
struct hashtableNode{
    T              value;
    hashtableNode* next;

    hashtableNode() = default;
    hashtableNode( const T& n ) : value(n), next(nullptr){}

    hashtableNode( const hashtableNode& node ) : value( node.value), next(node.next){}
    hashtableNode( hashtableNode&& node ) : value( std::move(node.value)), next(node.next)
    {
        node.next = nullptr;
    }
};


// 迭代器
template<class T, class hashFun, class keyEqual>
struct iterator_base{
    using hashtable_type         = hashtable<T, hashFun, keyEqual>;
    using base                   = iterator_base<T, hashFun, keyEqual>;
    using node_ptr_type          = hashtableNode<T>*;
    using contain_ptr_type       = hashtable_type*;
    using const_node_ptr_type    = const node_ptr_type;
    using const_contain_ptr_type = const contain_ptr_type;
    using iterator_type          = iterator<T, hashFun, keyEqual>;
    using const_iterator_type    = const_iterator<T, hashFun, keyEqual>;

    node_ptr_type    node_ptr;  //迭代器指向的节点
    contain_ptr_type hashtable_ptr; //保持与容器的连接

    iterator_base() = default;

    bool operator==(const base& rhs)
    { return node_ptr == rhs.node_ptr; }

    bool operator!=(const base& rhs)
    { return node_ptr != rhs.node_ptr; }
};

template<class T, class hashFun, class keyEqual>
struct iterator : public iterator_base<T, hashFun, keyEqual>{
    using base                = iterator_base<T, hashFun, keyEqual>;
    using hashtable_type      = typename base::hashtable_type;
    using iterator_type       = typename base::iterator_type;
    using const_iterator_type = typename base::const_iterator_type;
    using node_ptr_type       = typename base::node_ptr_type;
    using contain_ptr_type    = typename base::contain_ptr_type;

    using value_traits_type = value_traits<T>;
    using value_type        = T;
    using value_ptr_type    = value_type*;
    using value_reference   = value_type&;

    using base::node_ptr;
    using base::hashtable_ptr;

    iterator() = default;

    iterator( node_ptr_type n, contain_ptr_type t )
    {
        node_ptr = n;
        hashtable_ptr = t;
    }

    iterator( const iterator& rhs )
    {
        node_ptr = rhs.node_ptr;
        hashtable_ptr = rhs.hashtable_ptr;

    }

    iterator( const const_iterator_type& rhs )
    {
        node_ptr = rhs.node_ptr;
        hashtable_ptr = rhs.hashtable_ptr;
    }


    iterator& operator=( const iterator& rhs )
    {
        if( this != &rhs )
        {
            node_ptr = rhs.node_ptr;
            hashtable_ptr = rhs.hashtable_ptr;
        }
        return *this;
    }

    iterator& operator=( const const_iterator_type& rhs )
    {
        if( this != &rhs )
        {
            node_ptr = rhs.node_ptr;
            hashtable_ptr = rhs.hashtable_ptr;
        }

        return *this;
    }

    value_reference operator*() const { return node_ptr -> value; }

    value_ptr_type operator->() const  { return &(operator*());}

    // 找到下一个非空的数据对
    iterator& operator++()
    {
        static_assert( node_ptr != nullptr, " node is nullptr ");
        const node_ptr_type old_ptr = node_ptr;
        node_ptr = node_ptr -> next;
        if( node_ptr == nullptr )  // 如果为空, 那么指向下一个非空bucket的起始处
        {
            auto index = hashtable_ptr -> hash(value_traits_type::get_key( old_ptr -> value ));

            while( !node_ptr && ++index < hashtable_ptr -> size )
                node_ptr = hashtable_ptr -> buckets[ index ];
        }
        return *this;
    }

    iterator operator++(int)
    {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }
};

template <class T, class hashFun, class keyEqual>
struct const_iterator : public iterator_base<T, hashFun, keyEqual> {
    using base                = iterator_base<T, hashFun, keyEqual>;
    using hashtable_type      = typename base::hashtable_type;
    using iterator_type       = typename base::iterator_type;
    using const_iterator_type = typename base::const_iterator_type;
    using node_ptr_type       = typename base::const_node_ptr_type;
    using contain_ptr_type    = typename base::const_contain_ptr_type;

    using value_traits_type = value_traits<T>;
    using value_type        = T;
    using value_ptr_type    = const value_type*;
    using value_reference   = const value_type&;

    using base::node_ptr;
    using base::hashtable_ptr;

    const_iterator() = default;
    const_iterator( node_ptr_type n, contain_ptr_type t )
    {
        node_ptr      = n;
        hashtable_ptr = t;
    }


    const_iterator( const iterator_type& rhs )
    {
        node_ptr      = rhs.node_ptr;
        hashtable_ptr = rhs.hashtable_ptr;
    }

    const_iterator( const const_iterator_type & rhs )
    {
        node_ptr      = rhs.node_ptr;
        hashtable_ptr = rhs.hashtable_ptr;
    }

    const_iterator& operator=( const iterator_type& rhs )
    {
        if( this != &rhs )
        {
            node_ptr      = rhs.node_ptr;
            hashtable_ptr = rhs.hashtable_ptr;
        }
        return *this;
    }

    const_iterator_type operator=( const const_iterator_type & rhs )
    {
        if( this != &rhs )
        {
            node_ptr      = rhs.node_ptr;
            hashtable_ptr = rhs.hashtable_ptr;
        }
        return *this;
    }

    value_reference operator*() const { return node_ptr -> value;}
    value_ptr_type operator->() const { return &(operator*()); }

    const_iterator_type& operator++()
    {
        static_assert( this != nullptr , "this pointer is nullptr");
        const node_ptr_type old = node_ptr;

        node_ptr = node_ptr -> next;

        if( node_ptr == nullptr )
        {
            auto index = hashtable_ptr -> hash(value_traits_type::get_key( old -> value ));

            while( !node_ptr && ++index < hashtable_ptr -> size )
                node_ptr = hashtable_ptr -> buckets[ index ];
        }
        return *this;
    }

    const_iterator_type operator++( int )
    {
        const_iterator_type tmp = *this;
        ++*this;
        return tmp;
    }
};

template <class T>
struct local_iterator {
    using value_type      = T;
    using pointer_type    = T*;
    using reference_type  = T&;
    using difference_type = std::ptrdiff_t;
    using node_ptr_type   = hashtableNode<T>*;

    using self                      = local_iterator<T>;
    using local_iterator_type       = local_iterator<T>;
    using const_local_iterator_type = const_local_iterator<T>;

    node_ptr_type node_ptr;

    local_iterator( node_ptr_type n ):node_ptr(n) {}

    local_iterator(const local_iterator_type& rhs ):node_ptr(rhs.node_ptr) {}

    local_iterator(const const_local_iterator_type& rhs ): node_ptr(rhs.node_ptr) {}

    reference_type operator*() const { return node_ptr -> value; };
    pointer_type operator->() const { return &(operator*());}

    self& operator++()
    {
        static_assert( node_ptr != nullptr , "this pointer is nullptr");
        node_ptr = node_ptr -> next;
        return *this;
    }

    self operator++( int )
    {
        self tmp{*this};
        ++*this;
        return tmp;
    }

    bool operator==( const self& rhs ) const { return node_ptr == rhs.node_ptr; }
    bool operator!=( const self& rhs ) const { return node_ptr != rhs.node_ptr; }
};

template <class T>
struct const_local_iterator {
    using value_type      = T;
    using pointer_type    = const T*;
    using reference_type  = const T&;
    using difference_type = std::ptrdiff_t;
    using node_ptr_type   = const hashtableNode<T>*;

    using self                      = const_local_iterator<T>;
    using local_iterator_type       = local_iterator<T>;
    using const_local_iterator_type = const_local_iterator<T>;

    node_ptr_type node_ptr;

    const_local_iterator( node_ptr_type n ):node_ptr(n) {}

    const_local_iterator(const local_iterator_type& rhs ):node_ptr(rhs.node_ptr) {}

    const_local_iterator(const const_local_iterator_type& rhs ): node_ptr(rhs.node_ptr) {}

    reference_type operator*() const { return node_ptr -> value; };
    pointer_type operator->() const { return &(operator*());}

    self& operator++()
    {
        static_assert( node_ptr != nullptr , "this pointer is nullptr");
        node_ptr = node_ptr -> next;
        return *this;
    }

    self operator++( int )
    {
        self tmp{*this};
        ++*this;
        return tmp;
    }

    bool operator==( const self& rhs ) const { return node_ptr == rhs.node_ptr; }
    bool operator!=( const self& rhs ) const { return node_ptr != rhs.node_ptr; }
};

static constexpr unsigned long long PRIME_NUM = 99;
// bucket 使用的大小
static constexpr size_t prime_list[] = {
  101ull, 173ull, 263ull, 397ull, 599ull, 907ull, 1361ull, 2053ull, 3083ull,
  4637ull, 6959ull, 10453ull, 15683ull, 23531ull, 35311ull, 52967ull, 79451ull,
  119179ull, 178781ull, 268189ull, 402299ull, 603457ull, 905189ull, 1357787ull,
  2036687ull, 3055043ull, 4582577ull, 6873871ull, 10310819ull, 15466229ull,
  23199347ull, 34799021ull, 52198537ull, 78297827ull, 117446801ull, 176170229ull,
  264255353ull, 396383041ull, 594574583ull, 891861923ull, 1337792887ull,
  2006689337ull, 3010034021ull, 4515051137ull, 6772576709ull, 10158865069ull,
  15238297621ull, 22857446471ull, 34286169707ull, 51429254599ull, 77143881917ull,
  115715822899ull, 173573734363ull, 260360601547ull, 390540902329ull,
  585811353559ull, 878717030339ull, 1318075545511ull, 1977113318311ull,
  2965669977497ull, 4448504966249ull, 6672757449409ull, 10009136174239ull,
  15013704261371ull, 22520556392057ull, 33780834588157ull, 50671251882247ull,
  76006877823377ull, 114010316735089ull, 171015475102649ull, 256523212653977ull,
  384784818980971ull, 577177228471507ull, 865765842707309ull, 1298648764060979ull,
  1947973146091477ull, 2921959719137273ull, 4382939578705967ull, 6574409368058969ull,
  9861614052088471ull, 14792421078132871ull, 22188631617199337ull, 33282947425799017ull,
  49924421138698549ull, 74886631708047827ull, 112329947562071807ull, 168494921343107851ull,
  252742382014661767ull, 379113573021992729ull, 568670359532989111ull, 853005539299483657ull,
  1279508308949225477ull, 1919262463423838231ull, 2878893695135757317ull, 4318340542703636011ull,
  6477510814055453699ull, 9716266221083181299ull, 14574399331624771603ull, 18446744073709551557ull
};

inline size_t next_prime( size_t n )
{
    const size_t* first = prime_list;
    const size_t* last = prime_list + PRIME_NUM;
    const size_t* pos = std::lower_bound(first, last, n);
    return pos  == last ? *(last-1) : *pos;
}



template<class T, class hashFun, class keyEqual>
class hashtable{
    friend struct iterator<T, hashFun, keyEqual>;
    friend struct const_iterator<T, hashFun, keyEqual>;

    using value_traits_type = value_traits<T>;
    using key_type          = typename value_traits_type::key_type;
    using mapped_type       = typename value_traits_type::mapped_type;
    using value_type        = typename value_traits_type::value_type;
    using hasher            = hashFun;
    using key_equal         = keyEqual;


    using node_type     = hashtableNode<T>;
    using node_ptr_type = node_type*;
    using bucket_type   = std::vector<node_ptr_type>;

    using allocator_type      = std::allocator<T>;
    using data_allocator_type = std::allocator<T>;
    using node_allocator_type = std::allocator<node_type>;

    using pointer_type         = typename allocator_type::pointer;
    using const_pointer_type   = typename allocator_type::const_pointer;
    using reference_type       = typename allocator_type::reference;
    using const_reference_type = typename allocator_type::const_reference;
    using size_type            = typename allocator_type::size_type;
    using difference_type      = typename allocator_type::difference_type;

    using iterator             = iterator<T, hashFun, keyEqual>;
    using const_iterator       = const_iterator<T, hashFun, keyEqual> ;
    using local_iterator       = local_iterator<T> ;
    using const_local_iterator = const_local_iterator<T> ;


    using cv_key_type = typename std::remove_cv<typename T::first_type>::type;
  private:
    hashtableNode<T>*               node_ptr;
    std::vector<hashtableNode<T> *> buckets;
    size_t                          m_size;
    size_t                          m_bucket_size;
    float                           m_load_factor;
    hasher                          m_hash;
    keyEqual                        equal;
  private:
    bool is_equal( const cv_key_type& key1, const cv_key_type& key2  )
    {
        return equal( key1, key2 );
    }

    bool is_equal( const cv_key_type& key1, const cv_key_type& key2  ) const
    {
        return equal( key1, key2 );
    }

    const_iterator M_cit( node_ptr_type node ) const noexcept
    {
        return const_iterator(node, const_cast<hashtable*>(this));
    }

    iterator M_begin() noexcept
    {
        for( size_t n = 0; n < m_size; ++n )
            if( buckets[ n ] )
                return iterator(buckets[ n ], this );

        return iterator( nullptr, this );
    }


    const_iterator M_begin() const noexcept
    {
        for( size_t n = 0; n < m_size; ++n )
            if( buckets[ n ] )
                return M_cit( buckets[ n ] );

        return M_cit( nullptr );
    }
  public:
    explicit hashtable( size_t bucket_count,
                        const hashFun& hashfun = hashFun(),
                        const keyEqual& equal = keyEqual() )
            :m_size( 0 ), m_load_factor( 1.0f ), m_hash(hashfun), equal(equal)
    {
        init( bucket_count );
    }

    hashtable( const hashtable& rhs )
            :m_hash(rhs.m_hash), equal( rhs.equal )
    {
        copy_init( rhs );
    }

    hashtable( hashtable&& rhs ) noexcept
            :m_bucket_size(rhs.m_bucket_size),
             m_size(rhs.m_size),
             m_load_factor(rhs.m_load_factor),
             m_hash(rhs.m_hash),
             equal(rhs.equal)
    {
        buckets         = move( rhs.buckets );
        rhs.m_bucket_size = 0;
        rhs.m_size        = 0;
        rhs.m_load_factor = 0.0f;
    }

    hashtable& operator=(const hashtable& rhs )
    {
        if( this != &rhs )
        {
            hashtable tmp( rhs );
            swap( tmp );
        }
        return *this;
    }

    hashtable& operator=(hashtable&& rhs ) noexcept
    {
        hashtable tmp( std::move( rhs ));
        swap( tmp );
        return *this;
    }

    ~hashtable() { clear(); }

    iterator begin() noexcept
    { return M_begin(); }
    const_iterator begin() const noexcept
    { return M_begin(); }
    iterator end() noexcept
    { return iterator( nullptr, this ); }
    const_iterator end() const noexcept
    { return M_cit( nullptr ); }

    const_iterator cbegin() const noexcept
    { return begin(); }
    const_iterator cend() const noexcept
    { return end(); }

    // 容器相关的操作
    bool empty() const noexcept { return m_size == 0; }
    size_t size() const noexcept { return m_size; }
    size_t max_size() const noexcept { return static_cast<size_t>(-1); }

    // 修改容器
    template<class ...Args>
    iterator emplace_multi( Args&& ...args )
    {
        auto np = create_node(std::forward<Args>(args)...);
        if( static_cast<float>(m_size + 1 )  > static_cast<float>(m_bucket_size) * max_load_factor() ) {
            rehash( m_size + 1 );
            return insert_node_multi( np );
        }
        else {
            destroy_node( np );
            return iterator();
        }
    }

    template <class ... Args>
    std::pair<iterator, bool> emplace_unique( Args&& ... args )
    {
        auto np = create_np(std::forward<Args>(args)...);
        if( static_cast<float>(m_size + 1 ) > static_cast<float>(m_bucket_size ) * max_load_factor() ) {
            rehash( m_size + 1 );
            return insert_node_unique(np);
        }
        else {
            destroy_node(np);
            return std::make_pair(iterator(np, this), false);
        }
    }

    template <class... Args>
    iterator emplace_multi_use_hint(const_iterator /*hint*/, Args &&...args)
    {
      return emplace_multi(std::forward<Args>(args)...);
    }

    template <class... Args>
    iterator emplace_unique_use_hint(const_iterator /*hint*/, Args &&...args)
    {
      return emplace_unique(std::forward<Args>(args)...).first;
    }

    iterator insert_multi_noresize( const value_type& value )
    {
        const auto n = hash( value_traits_type::get_key( value ));
        auto first = buckets[ n ];
        auto tmp = create_node( value );
        for( auto cur = first; cur; cur = cur -> next )
            if( is_equal(value_traits_type::get_key(cur -> value ), value_traits_type::get_key(value)))
            {
                tmp -> next = cur -> next;
                cur -> next = tmp;
                ++m_size;
                return iterator( tmp, this );
            }

        tmp -> next = first;
        buckets[ n ] = tmp;
        ++m_size;
        return iterator(tmp, this );
    }
    std::pair<iterator, bool> insert_unique_noresize( const value_type& value )
    {
        const auto n = hash( value_traits_type::get_key( value ));
        auto first = buckets[ n ];
        for( auto cur = first; cur; cur = cur -> next )
            if( is_equal( value_traits_type::get_key( cur -> value), value_traits_type::get_key(value)))
                return std::make_pair(iterator(cur, this), false);

        auto tmp = create_node( value );
        tmp -> next = first;
        buckets[ n ] = tmp;
        ++m_size;
        return std::make_pair(iterator(tmp, this), true);

    }

    iterator insert_multi( const value_type& value )
    {
        rehash_if_need( 1 );
        return insert_multi_noresize(value);
    }

    iterator insert_multi(value_type&& value)
    {
        return emplace_multi(std::move(value));
    }

    std::pair<iterator, bool> insert_unique( const value_type& value )
    {
        rehash_if_need( 1 );
        return insert_unique_noresize(value);
    }

    std::pair<iterator, bool> insert_unique( value_type&& value )
    {
        return emplate_unique( std::move( value ));
    }

    iterator insert_multi_use_hint(const_iterator /*hint*/,
                                   const value_type &value)
    {
      return insert_multi(value);
    }
    iterator insert_multi_use_hint(const_iterator /*hint*/,
                                   value_type &&value)
    {
        return emplace_multi(std::move(value));
    }

    iterator insert_unique_use_hint(const_iterator /*hint*/,
                                    const value_type &value)
    {
      return insert_unique(value).first;
    }
    iterator insert_unique_use_hint(const_iterator /*hint*/,
                                    value_type &&value)
    {
      return emplace_unique(std::move(value));
    }

    template<class T1>
    void insert_multi( T1 first, T1 last )
    {
        // .copy_insert_multi( first, last, iterator_category(first));
        copy_insert_multi( first, last );
    }

    template<class T1>
    void insert_unique(T1 first, T1 last)
    {
        // .copy_insert_unique(first, last, iterator_category(first));
        copy_insert_unique(first, last);
    }

    //erase /clear
    void erase( const_iterator position )
    {
        auto p = position.node;
        if( p ){
            const auto n = hash( value_traits_type::get_key( p -> value));
            auto cur = buckets[ n ];
            if( cur == p ) {
                buckets[ n ] = cur -> next;
                destroy_node(cur);
                --m_size;
            }else{
                auto next = cur -> next;
                while( next ){
                    if( next == p ) {
                        cur -> next = next -> next;
                        destroy_node( next );
                        --m_size;
                        break;
                    }
                    else {
                        cur = next;
                        next = cur -> next;
                    }
                }
            }
        }
    }

    void erase( const_iterator first, const_iterator last )
    {
        if( first.node == last.node )
            return;
        auto first_bucket = first.node
                            ? hash(value_traits_type::get_key(first.node->value))
                            : m_bucket_size;
        auto last_bucket = last.node
                           ? hash( value_traits_type::get_key(last.node -> value ))
                           : m_bucket_size;

        if( first_bucket == last_bucket ){
            erase_bucket( first_bucket, first.node, last.node );
        }
        else {
            erase_bucket(first_bucket, first.node, nullptr );
            for( auto n = first_bucket + 1; n < last_bucket; ++n ) {
                if( buckets[ n ] != nullptr )
                    erase_bucket(n, nullptr );
            }
            if( last_bucket != m_bucket_size )
                erase_bucket(last_bucket, last.node);
        }
    }

    size_t erase_multi( const key_type& key )
    {
        auto p = equal_range_multi(key);
        if( p.first.node != nullptr ){
            erase(p.first, p.second);
            return std::distance(p.first, p.second);
        }
        return 0;
    }

    size_t erase_unique( const key_type& key )
    {
        const auto n = hash( key );
        auto first = buckets[ n ];
        if( first ){
            if( is_equal(value_traits_type::get_key(first -> value), key)) {
                    buckets[ n ] = first -> next;
                    destroy_node(first);
                    --m_size;
                    return -1;
            }else{
                auto next = first -> next;
                while( next ) {
                    if( is_equal(value_traits_type::get_key(next->value), key)){
                        first -> next = next -> next;
                        destroy_node( next );
                        --m_size;
                        return 1;
                    }
                    first = next;
                    next = first->next;
                }
            }
        }
        return 0;
    }

    void clear()
    {
        if( m_size != 0 ) {
            for( size_t i = 0; i < m_bucket_size; ++i ) {
                node_ptr_type cur = buckets[ i ];
                while( cur != nullptr ) {
                    node_ptr_type next = cur -> next;
                    destroy_node( cur );
                    cur = next;
                }
                buckets[ i ] = nullptr;
            }
            m_size = 0;
        }
    }

    void swap( hashtable& rhs ) noexcept
    {
        if( this != &rhs ){
            buckets.swap(rhs.buckets);
            std::swap(m_bucket_size, rhs.m_bucket_size);
            std::swap(m_size, rhs.size);
            std::swap(m_load_factor, rhs.m_load_factor);
            std::swap(m_hash, rhs.m_hash);
            std::swap(equal, rhs.equal);
        }
    }

    // 查找
    size_t count( const key_type& key ) const
    {
        const auto n = hash( key );
        size_t result = 0;
        for( node_ptr_type cur = buckets[ n ]; cur; cur = cur -> next ) {
            if( is_equal(value_traits_type::get_key(cur -> valoue ), key )) {
                ++result;
            }
        }

        return result;
    }

    iterator find( const key_type& key )
    {
        const auto n = hash( key );
        node_ptr_type first = buckets[ n ];
        for(; first && !is_equal(value_traits_type::get_key(first->value), key); first = first -> next ) {}
        return iterator( first, this );
    }

    const_iterator find( const key_type& key ) const
    {
        const auto n = hash( key );
        node_ptr_type first = buckets[ n ];
        for(; first && !is_equal(value_traits_type::get_key(first->value), key); first = first->next ) {}
        return M_cit(first);
    }

    std::pair<iterator, iterator> equal_range_multi( const key_type& key )
    {
        const auto n = hash( key );

        for( node_ptr_type first = buckets[ n ]; first; first = first -> next ) {
            if( is_equal( value_traits_type::get_key(first -> value), key)) {
                for( node_ptr_type second = first -> next; second ; second = second -> next ) {
                    if( !is_equal(value_traits_type::get_key(second -> value), key))
                        return std::make_pair(iterator(first, this), iterator(second, this));
                }
                for( auto m = n + 1; m < m_bucket_size; ++m ) {
                    if( buckets[ m ] )
                        return std::make_pair(iterator(first, this), end());
                }
                return std::make_pair(iterator(first, this), end());
            }
        }

        return std::make_pair(end(), end());
    }

    std::pair<const_iterator, const_iterator> equal_range_multi(const key_type& key) const
    {
        const auto n = hash( key );
        for( node_ptr_type first = buckets[n]; first; first = first -> next ) {
            if( is_equal(value_traits_type::get_key(first -> valuek), key)) {
                for( node_ptr_type second = first -> next; second; second = second -> next ) {
                    if( !is_equal(value_traits_type::get_key(second -> value), key))
                        return std::make_pair(M_cit(first), M_cit(second));
                }

                for( auto m = n+ 1; m < m_bucket_size; ++m ) {
                    if( buckets[ m ] )
                        return std::make_pair(M_cit(first), M_cit(buckets[m]));
                }

                return std::make_pair(M_cit(first), cend());
            }
        }
        return std::make_pair(cend(),cend());
    }

    std::pair<iterator, iterator> equal_range_unique( const key_type& key )
    {
        const auto n = hash( key );
        for( node_ptr_type first = buckets[ n ]; first; first = first -> next ) {
            if( is_equal(value_traits_type::get_key(first->value), key)) {
                if( first -> next )
                    return std::make_pair(iterator(first, this), iterator(first -> next, this) );

                for( auto m = n + 1; m < m_bucket_size; ++m ) {
                    if( buckets[m])
                        return std::make_pair(iterator(first, this), iterator(buckets[m], this));
                }
                return std::make_pair(iterator(first,this),  end());
            }
        }

        return std::make_pair(end(), end());
    }

    std::pair<const_iterator, const_iterator> equal_range_unique(const key_type& key ) const
    {
        const auto n = hash( key );
        for( node_ptr_type first = buckets[n]; first; first = first -> next ) {
            if( is_equal(value_traits_type::get_key(first->value), key)) {
                if( first -> next )
                    return std::make_pair(M_cit(first), M_cit(buckets[first -> next]));

                for( auto m = n +1; m < m_bucket_size; ++m ) {
                    if( buckets[m])
                        return std::make_pair(M_cit(first), M_cit(buckets[m]));
                }

                return std::make_pair(M_cit(first),cend());
            }
        }
        return std::make_pair(cend(), cend());
    }

    // bucket interface

    local_iterator begin( size_t n ) noexcept
    {
        static_assert(n < m_size, "error: n > m_size");
        return buckets[n];
    }

    local_iterator end( size_t n ) noexcept
    {
        static_assert( n < m_size, "error: n > m_size");
        return nullptr;
    }

    const_local_iterator begin( size_t n ) const noexcept
    {
        static_assert( n < m_size, "error: n > m_size");
        return buckets[n];
    }
    const_local_iterator cbegin( size_t n ) const noexcept
    {
        static_assert( n < m_size, "error: n > m_size");
        return buckets[ n ];
    }

    const_local_iterator end( size_t n ) const noexcept
    {
        static_assert( n < m_size, "error: n > m_size");
        return nullptr;
    }

    const_local_iterator cend( size_t n ) const noexcept
    {
        static_assert( n < m_size, "error: n > m_size");
        return nullptr;
    }

    size_t bucket_count() const noexcept
    {
        return m_bucket_size;
    }
    size_t max_bucket_count() const noexcept
    {
        return prime_list[PRIME_NUM - 1];
    }

    size_t bucket_size( size_t n ) const noexcept
    {
        size_t result = 0;
        for( auto cur = buckets[n]; cur; cur = cur -> next )
            ++result;

        return result;
    }

    size_t bucket( const key_type& key ) const
    {
        return hash(key);
    }

    //hash policy
    float load_factor() const noexcept
    {
        return m_bucket_size != 0 ? static_cast<float>(m_size) / m_bucket_size : 0.0f;
    }

    float max_load_factor() const noexcept
    {
        return m_load_factor;
    }

    void max_load_factor( float n )
    {
        // const int a = 0;
        // static_assert( n < a, "invalid hash load factor" );
        m_load_factor = n;
    }

    void rehash( size_t count )
    {
        auto n = next_prime( count );
        if( n > m_bucket_size ){
            replace_bucket( n );
        }
        else {
            if( static_cast<float>(m_size)/static_cast<float>(n) < max_load_factor() - 0.25f &&
                static_cast<float>(m_bucket_size)*0.75)
                replace_bucket(n);
        }
    }

    void reserve(size_t count )
    {
        rehash(static_cast<size_t>((float)count / max_load_factor() + 0.5f));
    }
    hasher hash_fcn() const
    {
        return m_hash;
    }
    key_equal key_eq() const
    {
        return equal;
    }

  private:
    void init( size_t n )
    {
        const auto bucket_nums = next_size(n);
        try{
          buckets.reserve(bucket_nums);
          buckets.assign(bucket_nums, nullptr);
        }
        catch(...)
        {
            m_bucket_size = 0;
            m_size = 0;
            throw;
        }
        m_bucket_size = buckets.size();
    }

    void copy_init( const hashtable& ht )
    {
        m_bucket_size = 0;
        buckets.reserve(ht.m_bucket_size);
        buckets.assign(ht.m_bucket_size, nullptr);
        try{
            for( size_t i = 0; i < ht.m_bucket_size; ++i ){
                node_ptr_type cur = ht.buckets[i];
                if( cur ) {
                    auto copy = create_node( cur -> value );
                    buckets[ i ] = copy;
                    for( auto next = cur -> next; next; cur = next, next = cur -> next ) {
                        copy -> next = create_node( next -> value );
                        copy = copy -> next;
                    }
                    copy -> next = nullptr;
                }
            }
            m_bucket_size = ht.m_bucket_size;
            m_load_factor = ht.m_load_factor;
            m_size = ht.m_size;
        }
        catch( ... )
        {
            clear();
        }
    }

    template<class ... Args >
    node_ptr_type create_node(Args&& ... args )
    {
        node_ptr_type tmp = node_allocator_type::allocate(1);
        try{
            data_allocator_type::construct(std::addressof(tmp->value), std::forward<Args>(args)...);
            tmp -> next = nullptr;
        }
        catch(...){
            node_allocator_type::deallocate(tmp);
            throw;
        }

        return tmp;
    }

    void destroy_node(node_ptr_type node )
    {
        data_allocator_type::destroy(std::addressof(node->value));
        node_allocator_type::deallocate(node);
        node = nullptr;
    }

    size_t next_size( size_t n ) const
    {
        return next_prime(n);
    }

    size_t hash( const key_type& key, size_t n ) const
    {
        return m_hash(key) % n;
    }

    size_t hash( const key_type& key ) const
    {
        return m_hash(key) % m_bucket_size;
    }

    void rehash_if_need(size_t n )
    {
        if( static_cast<float>(m_size + n ) > static_cast<float>(m_bucket_size) * max_load_factor() )
            rehash( m_size + n );
    }


    //insert
    template<class T1>
    // void copy_insert_multi( T1 first, T1 last, )
    void copy_insert_multi( T1 first, T1 last )
    {
        rehash_if_need(std::distance( first, last));
        for(;first != last; ++first)
            insert_multi_noresize(*first);
    }

    // template<class T1>
    // void copy_insert_multi(T1 first, T1 last, )
    // {
    //     size_t n = std::distance(first,  last);
    //     rehash_if_need(n);
    //     for(; n > 0; --n, ++first)
    //         insert_multi_noresize(*first);
    // }

    template <class T1>
    // void copy_insert_unique(T1 first, T1 last, )
    void copy_insert_unique(T1 first, T1 last )
    {
        rehash_if_need(std::distance(first,  last));
        for(; first != last; ++first )
            insert_unique_noresize(*first);
    }

    // template<class T1>
    // void copy_insert_unique(T1 first, T1 last, )
    // {
    //     size_t n = std::distance(first,  last);
    //     rehash_if_need(n);
    //     for(; n > 0; --n, ++first)
    //         insert_unique_noresize(*first);
    // }

    std::pair<iterator, bool> insert_node_unique(node_ptr_type np )
    {
        const auto n = hash(value_traits_type::get_key( np -> value));
        auto cur = buckets[ n ];
        if( cur == nullptr ) {
            buckets[ n ] = np;
            ++m_size;
            return std::make_pair(iterator(np, this), true);
        }

        for(; cur; cur = cur -> next ) {
            if( is_equal( value_traits_type::get_key(cur -> value), value_traits_type::get_key(np -> value)))
                return std::make_pair( iterator(cur, this), false);
        }

        np -> next  = buckets[ n ];
        buckets[ n ] = np;
        ++m_size;
        return std::make_pair(iterator(np, this), true);
    }

    void replace_bucket(size_t bucket_count )
    {
        bucket_type bucket( bucket_count );
        if( m_size != 0 ) {
            for( size_t i = 0; i < m_bucket_size; ++i ) {
                for( auto first = buckets[ i ]; first; first = first -> next ) {
                    auto tmp = create_node( first -> value );
                    const auto n = hash( value_traits_type::get_key(first->value), bucket_count );
                    auto f = buckets[ n ];
                    bool is_inserted = false;
                    for( auto cur = f; cur; cur = cur -> next ) {
                        if( is_equal( value_traits_type::get_key(cur ->value), value_traits_type::get_key(first -> value))) {
                            tmp -> next = cur -> next;
                            cur -> next  = tmp;
                            is_inserted = true;
                            break;
                        }
                    }
                    if( !is_inserted ){
                        tmp -> next = f;
                        buckets[ n ] = tmp;
                    }
                }
            }
        }
        buckets.swap( bucket );
        m_bucket_size = buckets.m_size();
    }

    void erase_bucket(size_t n, node_ptr_type first, node_ptr_type last )
    {
        auto cur = buckets[ n ];
        if( cur == first ){
            erase_bucket(n , last);
        }
        else {
            node_ptr_type next = cur -> next;
            for(; next != first; cur = next, next = cur -> next ) {}
            while( next != last ) {
                cur -> next = next -> next;
                destroy_node(next );
                next = cur -> next;
                --m_size;
            }
        }
    }

    void erase_bucket(size_t n, node_ptr_type last)
    {
        auto cur = buckets[ n ];
        while( cur != last ) {
            auto next = cur -> next;
            destroy_node( cur );
            cur = next;
            --m_size;
        }
        buckets[n] = last;
    }

    bool equal_to_multi( const hashtable& other )
    {
        if( m_size != other.m_size )
            return false;
        for( auto f = begin(), l = end(); f != l; ) {
            auto p1 = eqaul_range_multi(value_traits_type::get_key(*f));
            auto p2 = other.eqaul_range_multi(value_traits_type::get_key(*f));
            if( std::distance(p1.first, p1.last) != std::distance(p2.first,  p2.last) ||
                !std::is_permutation(p1.first, p2.last, p2.first, p2.last))
                return false;
            f = p1.last;
        }
        return true;
    }

    bool equal_to_unique(const hashtable& other)
    {
        if( m_size != other.m_size )
            return false;

        for( auto f = begin(), l = end(); f != l; ++f ) {
            auto res = other.find( value_traits_type::get_key(*f));
            if( res.node == nullptr || *res != *f )
                return false;
        }
        return true;
    }
};

template <class T, class hashFun, class keyEqual>
void swap( hashtable<T, hashFun, keyEqual> & lhs,
           hashtable<T, hashFun, keyEqual>& rhs ) noexcept
{
    lhs.swap(rhs);
}

int main()
{
    return 0;
}
