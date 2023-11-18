#MyTinySTL

template <class T, class Hash, class KeyEqual>
class hashtable
{
  friend struct mystl::ht_iterator<T, Hash, KeyEqual>;
  friend struct mystl::ht_const_iterator<T, Hash, KeyEqual>;

public:
  // hashtable 的型别定义
  typedef ht_value_traits<T>                          value_traits;
  typedef typename value_traits::key_type             key_type;
  typedef typename value_traits::mapped_type          mapped_type;
  typedef typename value_traits::value_type           value_type;
  typedef Hash                                        hasher;
  typedef KeyEqual                                    key_equal;

  typedef hashtable_node<T>                           node_type;
  typedef node_type*                                  node_ptr;
  typedef mystl::vector<node_ptr>                     bucket_type;

  typedef mystl::allocator<T>                         allocator_type;
  typedef mystl::allocator<T>                         data_allocator;
  typedef mystl::allocator<node_type>                 node_allocator;

  typedef typename allocator_type::pointer            pointer;
  typedef typename allocator_type::const_pointer      const_pointer;
  typedef typename allocator_type::reference          reference;
  typedef typename allocator_type::const_reference    const_reference;
  typedef typename allocator_type::size_type          size_type;
  typedef typename allocator_type::difference_type    difference_type;

  typedef mystl::ht_iterator<T, Hash, KeyEqual>       iterator;
  typedef mystl::ht_const_iterator<T, Hash, KeyEqual> const_iterator;
  typedef mystl::ht_local_iterator<T>                 local_iterator;
  typedef mystl::ht_const_local_iterator<T>           const_local_iterator;

  allocator_type get_allocator() const { return allocator_type(); }

private:
  // 用以下六个参数来表现 hashtable
  bucket_type buckets_;
  size_type   bucket_size_;
  size_type   size_;
  float       mlf_;
  hasher      hash_;
  key_equal   equal_;

private:
    bool is_equal(const key_type& key1, const key_type& key2);

    bool is_equal(const key_type& key1, const key_type& key2) const;

    const_iterator M_cit(node_ptr node) const noexcept

  iterator M_begin() noexcept

  const_iterator M_begin() const noexcept

public:
  // 构造、复制、移动、析构函数
  explicit hashtable(size_type bucket_count,
                     const Hash& hash = Hash(),
                     const KeyEqual& equal = KeyEqual())
            :size_(0), mlf_(1.0f), hash_(hash), equal_(equal);
  {
    init(bucket_count);
  }

  template <class Iter, typename std::enable_if<
    mystl::is_input_iterator<Iter>::value, int>::type = 0>
    hashtable(Iter first, Iter last,
              size_type bucket_count,
              const Hash& hash = Hash(),
              const KeyEqual& equal = KeyEqual())
    :size_(mystl::distance(first, last)), mlf_(1.0f), hash_(hash), equal_(equal)
  {
    init(mystl::max(bucket_count, static_cast<size_type>(mystl::distance(first, last))));
  }

  hashtable(const hashtable& rhs)
    :hash_(rhs.hash_), equal_(rhs.equal_)
  {
    copy_init(rhs);
  }
  hashtable(hashtable&& rhs) noexcept
    : bucket_size_(rhs.bucket_size_),
    size_(rhs.size_),
    mlf_(rhs.mlf_),
    hash_(rhs.hash_),
    equal_(rhs.equal_)
  {
    buckets_ = mystl::move(rhs.buckets_);
    rhs.bucket_size_ = 0;
    rhs.size_ = 0;
    rhs.mlf_ = 0.0f;
  }

  hashtable& operator=(const hashtable& rhs);
  hashtable& operator=(hashtable&& rhs) noexcept;

  ~hashtable() { clear(); }

  // 迭代器相关操作
  iterator       begin()        noexcept
  const_iterator begin()  const noexcept
  iterator       end()          noexcept
  const_iterator end()    const noexcept

  const_iterator cbegin() const noexcept
  const_iterator cend()   const noexcept

  // 容量相关操作
  bool      empty()    const noexcept { return size_ == 0; }
  size_type size()     const noexcept { return size_; }
  size_type max_size() const noexcept { return static_cast<size_type>(-1); }

  // 修改容器相关操作

  // emplace / empalce_hint

  template <class ...Args>
  iterator emplace_multi(Args&& ...args);

  template <class ...Args>
  pair<iterator, bool> emplace_unique(Args&& ...args);

  // [note]: hint 对于 hash_table 其实没有意义，因为即使提供了 hint，也要做一次 hash，
  // 来确保 hash_table 的性质，所以选择忽略它
  template <class ...Args>
  iterator emplace_multi_use_hint(const_iterator /*hint*/, Args&& ...args)

  template <class ...Args>
  iterator emplace_unique_use_hint(const_iterator /*hint*/, Args&& ...args)

  // insert

  iterator             insert_multi_noresize(const value_type& value);
  pair<iterator, bool> insert_unique_noresize(const value_type& value);

  iterator insert_multi(const value_type& value)

  iterator insert_multi(value_type&& value)


  pair<iterator, bool> insert_unique(const value_type& value)
  pair<iterator, bool> insert_unique(value_type&& value)

  // [note]: 同 emplace_hint
  iterator insert_multi_use_hint(const_iterator /*hint*/, const value_type& value)
  iterator insert_multi_use_hint(const_iterator /*hint*/, value_type&& value)

  iterator insert_unique_use_hint(const_iterator /*hint*/, const value_type& value)
  iterator insert_unique_use_hint(const_iterator /*hint*/, value_type&& value)

  template <class InputIter>
  void insert_multi(InputIter first, InputIter last)

  template <class InputIter>
  void insert_unique(InputIter first, InputIter last)

  // erase / clear

  void      erase(const_iterator position);
  void      erase(const_iterator first, const_iterator last);

  size_type erase_multi(const key_type& key);
  size_type erase_unique(const key_type& key);

  void      clear();

  void      swap(hashtable& rhs) noexcept;

  // 查找相关操作

  size_type                            count(const key_type& key) const;

  iterator                             find(const key_type& key);
  const_iterator                       find(const key_type& key) const;

  pair<iterator, iterator>             equal_range_multi(const key_type& key);
  pair<const_iterator, const_iterator> equal_range_multi(const key_type& key) const;

  pair<iterator, iterator>             equal_range_unique(const key_type& key);
  pair<const_iterator, const_iterator> equal_range_unique(const key_type& key) const;

  // bucket interface

  local_iterator       begin(size_type n)        noexcept
  const_local_iterator begin(size_type n)  const noexcept
  const_local_iterator cbegin(size_type n) const noexcept

  local_iterator       end(size_type n)          noexcept
  const_local_iterator end(size_type n)    const noexcept
  const_local_iterator cend(size_type n)   const noexcept

  size_type bucket_count()                 const noexcept
  size_type max_bucket_count()             const noexcept

  size_type bucket_size(size_type n)       const noexcept;
  size_type bucket(const key_type& key)    const

  // hash policy

  float load_factor() const noexcept

  float max_load_factor() const noexcept
  void max_load_factor(float ml)

  void rehash(size_type count);

  void reserve(size_type count)

  hasher    hash_fcn() const { return hash_; }
  key_equal key_eq()   const { return equal_; }

private:
  // hashtable 成员函数

  // init
  void      init(size_type n);
  void      copy_init(const hashtable& ht);

  // node
  template  <class ...Args>
  node_ptr  create_node(Args&& ...args);
  void      destroy_node(node_ptr n);

  // hash
  size_type next_size(size_type n) const;
  size_type hash(const key_type& key, size_type n) const;
  size_type hash(const key_type& key) const;
  void      rehash_if_need(size_type n);

  // insert
  template <class InputIter>
  void copy_insert_multi(InputIter first, InputIter last, mystl::input_iterator_tag);
  template <class ForwardIter>
  void copy_insert_multi(ForwardIter first, ForwardIter last, mystl::forward_iterator_tag);
  template <class InputIter>
  void copy_insert_unique(InputIter first, InputIter last, mystl::input_iterator_tag);
  template <class ForwardIter>
  void copy_insert_unique(ForwardIter first, ForwardIter last, mystl::forward_iterator_tag);

  // insert node
  pair<iterator, bool> insert_node_unique(node_ptr np);
  iterator             insert_node_multi(node_ptr np);

  // bucket operator
  void replace_bucket(size_type bucket_count);
  void erase_bucket(size_type n, node_ptr first, node_ptr last);
  void erase_bucket(size_type n, node_ptr last);

  // comparision
  bool equal_to_multi(const hashtable& other);
  bool equal_to_unique(const hashtable& other);
};

# ME
template<class HashObj>
class HashTable {
  public:
    explicit HashTable( int size = 101 )
            : array( nextPrime( size ) ) { makeEmpty(); }

    bool contains( const HashObj & x ) const

    void makeEmpty()


    bool insert( const HashObj & x )


    bool insert( HashObj && x )


    bool remove( const HashObj & x )

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
    int findPos( const HashObj & x ) const
    void rehash()
    size_t myhash( const HashObj & x ) const


    int nextPrime( int n )

    bool isPrime( int n )
}
