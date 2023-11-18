#ifndef MYTINYSTL_UNORDERED_MAP_H_
#define MYTINYSTL_UNORDERED_MAP_H_

#include "hashtable.hh"
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <utility>

template<class Key, class T, class Hash, class keyEqual = std::equal_to<Key>>
class unordered_map{
  private:
    using base_type = hashtable<std::pair<const Key, T>, Hash, keyEqual>;
    base_type m_ht;
  public:
    using allocator_type = typename base_type::allocator_type;
    using key_type       = typename base_type::key_type;
    using mapped_type    = typename base_type::mapped_type;
    using value_type     = typename base_type::value_type;
    using hasher         = typename base_type::hasher;
    using key_equal      = typename base_type::key_equal;

    using difference_type      = typename base_type::difference_type;
    using pointer_type         = typename base_type::pointer_type;
    using const_pointer_type   = typename base_type::const_pointer_type;
    using reference_type       = typename base_type::reference_type;
    using const_reference_type = typename base_type::const_reference_type;

    using iterator             = typename base_type::iterator;
    using const_iterator       = typename base_type::const_iterator;
    using local_iterator       = typename base_type::local_iterator;
    using const_local_iterator = typename base_type::const_local_iterator;

    allocator_type get_allocator() const
    {
        return m_ht.get_allocator();
    }

  public:
    unordered_map()
            :m_ht(101, Hash(), keyEqual())
    {
    }

    explicit unordered_map(size_t bucket_count,
                           const Hash& hash = Hash(),
                           const keyEqual& equal = keyEqual())
            :m_ht(bucket_count, hash, equal){}

    template<class T1>
    unordered_map(T1 first, T1 last,
                  const size_t bucket_count = 101,
                  const Hash& hash = Hash(),
                  const keyEqual& equal = keyEqual())
            :m_ht(std::max(bucket_count, static_cast<size_t>(std::distance(first, last ))), hash, equal)
    {
        for(; first != last; ++first )
            m_ht.insert_unique_noresize(*first);
    }

    unordered_map( std::initializer_list<value_type> ilist,
                   const size_t bucket_count = 101,
                   const Hash& hash = Hash(),
                   const keyEqual& equal = keyEqual())
            :m_ht(std::max(bucket_count, static_cast<size_t>(ilist.size())), hash, equal)
    {
        for( auto first = ilist.begin(), last = ilist.end(); first != last; ++first)
            m_ht.insert_unique_noresize(*first);
    }


    unordered_map( const unordered_map& rhs)
            :m_ht(rhs.m_ht){}

    unordered_map(unordered_map&& rhs) noexcept
            :m_ht(std::move(rhs.m_ht)) {}

    unordered_map& operator=(const unordered_map& rhs)
    {
        m_ht = rhs.m_ht;
        return *this;
    }

    unordered_map& operator=(unordered_map&& rhs)
    {
        m_ht = std::move(rhs.m_ht);
        return *this;
    }

    unordered_map& operator=(std::initializer_list<value_type> ilist)
    {
        m_ht.clear();
        m_ht.reserve(ilist.size());
        for( auto first = ilist.begin(), last = ilist.end(); first != last; ++first )
            m_ht.insert_unique_noresize(*first);
        return *this;
    }

    ~unordered_map() = default;

    iterator begin() noexcept
    {
        return m_ht.begin();
    }

    iterator end() noexcept
    {
        return m_ht.end();
    }

    const_iterator begin() const noexcept
    {
        return m_ht.begin();
    }
    const_iterator end() const noexcept
    {
        return m_ht.end();
    }

    const_iterator cbegin() const noexcept
    {
        return m_ht.cbegin();
    }
    const_iterator cend() const noexcept
    {
        return m_ht.cend();
    }

    bool empty() const noexcept
    {
        return m_ht.empty();
    }

    size_t size() const noexcept
    {
        return m_ht.size();
    }

    size_t max_size() const noexcept
    {
        return m_ht.max_size();
    }

    template<class ...Args>
    std::pair<iterator, bool> emplace(Args&& ...args)
    {
        return m_ht.emplace_unique(std::forward<Args>(args)...);
    }

    template<class ...Args>
    iterator emplace_hint(const_iterator hint, Args&& ...args)
    {
        return m_ht.emplace_unique_use_hint(hint, std::forward<Args>(args)...);
    }

    std::pair<iterator, bool> insert(const value_type& value)
    {
        return m_ht.insert_unique(value);
    }

    std::pair<iterator, bool> insert(value_type&& value)
    {
        return m_ht.emplace_unique(std::move(value));
    }


    iterator insert( const_iterator hint, const value_type& value)
    {
        return m_ht.insert_unique_use_hint(hint, value);
    }
    iterator insert( const_iterator hint, value_type&& value)
    {
        return m_ht.emplace_unique_use_hint(hint, std::move(value));
    }

    template<class T1>
    void insert(T1 first, T1 last)
    {
        m_ht.insert_unique(first, last);
    }

    void erase(iterator it)
    {
        m_ht.erase(it);
    }

    void erase(iterator first, iterator last)
    {
        m_ht.erase(first, last);
    }

    size_t erase( const key_type& key)
    {
        return m_ht.erase_unique(key);
    }

    void clear()
    {
        m_ht.clear();
    }

    void swap(unordered_map& other) noexcept
    {
        m_ht.swap(other.m_th);
    }

    mapped_type& at( const key_type& key)
    {
        iterator it = m_ht.find(key);
        static_assert(it.node == nullptr,"unordered_map<Key, T> no such element exists" );
        return it -> second;
    }

    const mapped_type& at( const key_type& key ) const
    {
        iterator it = m_ht.find(key);
        static_assert(it.node == nullptr,"unordered_map<Key, T> no such element exists" );
        return it->second;
    }

    mapped_type& operator[](const key_type& key)
    {
        iterator it = m_ht.find( key );
        if( it.node == nullptr )
            it = m_ht.emplace_unique(key, T{}).first;
        return it -> second;
    }
    mapped_type& operator[](key_type&& key)
    {
        iterator it = m_ht.find(key);
        if( it.node==nullptr)
            it = m_ht.emplace_unique(std::move(key), T{}).first;
        return it->second;
    }

    size_t count( const key_type& key ) const
    {
        return m_ht.count(key);
    }

    iterator find(const key_type& key)
    {
        return m_ht.find(key);
    }

    const_iterator find( const key_type& key ) const
    {
        return m_ht.find(key);
    }

    std::pair<iterator, iterator> equal_range( const key_type& key)
    {
        return m_ht.equal_range_unique(key);
    }

    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
    {
        return m_ht.equal_range_unique(key);
    }

    local_iterator begin( size_t n ) noexcept
    {
        return m_ht.begin(n);
    }
    const_local_iterator begin(size_t n ) const noexcept
    {
        return m_ht.begin(n);
    }
    const_local_iterator cbegin(size_t n ) const noexcept
    {
        return m_ht.cbegin(n);
    }

    local_iterator end(size_t n ) noexcept
    {
        return m_ht.end(n);
    }
    const_local_iterator end( size_t n ) const noexcept
    {
        return m_ht.end(n);
    }
    const_local_iterator cend( size_t n ) const noexcept
    {
        return m_ht.cend(n);
    }

    size_t bucket_count() const noexcept
    {
        return m_ht.bucket_count();
    }
    size_t max_bucket_count() const noexcept
    {
        return m_ht.max_bucket_count();
    }

    size_t bucket_size(size_t n ) const noexcept
    {
        return m_ht.bucket_size(n);
    }
    size_t bucket( const key_type& key) const
    {
        return m_ht.bucket(key);
    }

    float load_factor() const noexcept
    {
        return m_ht.load_factor();
    }

    float max_load_factor() const noexcept
    {
        return m_ht.max_load_factor();
    }
    void max_load_factor( float mlf )
    {
        m_ht.max_load_factor(mlf);
    }

    void rehash( size_t count )
    {
        m_ht.rehash(count);
    }

    void reserve( size_t count )
    {
        m_ht.reserve(count);
    }

    hasher hash_fcn() const
    {
        return m_ht.hash_fcn();
    }

    key_equal key_eq() const
    {
        return m_ht.key_eq();
    }

  public:
    friend bool operator==(const unordered_map& lhs, const unordered_map& rhs )
    {
        return lhs.m_ht.equal_range_unique(rhs.m_ht);
    }

    friend bool operator!=( const unordered_map& lhs, const unordered_map& rhs )
    {
        return !lhs.m_ht.equal_range_unique(rhs.m_ht);
    }
};

template<class Key, class T, class Hash, class keyEqual>
bool operator==(const unordered_map<Key, T, Hash, keyEqual>& lhs,
                const unordered_map<Key, T, Hash, keyEqual>& rhs)
{
    return lhs == rhs;
}
template<class Key, class T, class Hash, class keyEqual>
bool operator!=(const unordered_map<Key, T, Hash, keyEqual>& lhs,
                const unordered_map<Key, T, Hash, keyEqual>& rhs)
{
    return lhs != rhs;
}

template<class Key, class T, class Hash, class keyEqual>
void swap(unordered_map<Key, T, Hash, keyEqual>& lhs,
          unordered_map<Key, T, Hash, keyEqual>& rhs)
{
    lhs.swap(rhs);
}



/////////////////////
template <class Key, class T, class Hash, class keyEqual = std::equal_to<Key>>
class unordered_multimap{
  private:
    using base_type = hashtable<std::pair<const Key, T>, Hash, keyEqual>;
    base_type m_ht;

  public:
    using allocator_type = typename base_type::allocator_type;
    using key_type       = typename base_type::key_type;
    using mapped_type    = typename base_type::mapped_type;
    using value_type     = typename base_type::value_type;
    using hasher         = typename base_type::hasher;
    using key_equal      = typename base_type::key_equal;

    using difference_type      = typename base_type::difference_type;
    using pointer_type         = typename base_type::pointer_type;
    using const_pointer_type   = typename base_type::const_pointer_type;
    using reference_type       = typename base_type::reference_type;
    using const_reference_type = typename base_type::const_reference_type;

    using iterator             = typename base_type::iterator;
    using const_iterator       = typename base_type::const_iterator;
    using local_iterator       = typename base_type::local_iterator;
    using const_local_iterator = typename base_type::const_local_iterator;

    allocator_type get_allocator() const
    {
        return m_ht.get_allocator();
    }

  public:

    unordered_multimap()
            :m_ht(101, Hash(), keyEqual())
    {
    }

    explicit unordered_multimap(size_t bucket_count,
                           const Hash& hash = Hash(),
                           const keyEqual& equal = keyEqual())
            :m_ht(bucket_count, hash, equal){}

    template<class T1>
    unordered_multimap(T1 first, T1 last,
                  const size_t bucket_count = 101,
                  const Hash& hash = Hash(),
                  const keyEqual& equal = keyEqual())
            :m_ht(std::max(bucket_count, static_cast<size_t>(std::distance(first, last ))), hash, equal)
    {
        for(; first != last; ++first )
            m_ht.insert_multi_noresize(*first);
    }

    unordered_multimap( std::initializer_list<value_type> ilist,
                   const size_t bucket_count = 101,
                   const Hash& hash = Hash(),
                   const keyEqual& equal = keyEqual())
            :m_ht(std::max(bucket_count, static_cast<size_t>(ilist.size())), hash, equal)
    {
        for( auto first = ilist.begin(), last = ilist.end(); first != last; ++first)
            m_ht.insert_multi_noresize(*first);
    }


    unordered_multimap( const unordered_multimap& rhs)
            :m_ht(rhs.m_ht){}

    unordered_multimap(unordered_multimap&& rhs) noexcept
            :m_ht(std::move(rhs.m_ht)) {}

    unordered_multimap& operator=(const unordered_multimap& rhs)
    {
        m_ht = rhs.m_ht;
        return *this;
    }

    unordered_multimap& operator=(unordered_multimap&& rhs)
    {
        m_ht = std::move(rhs.m_ht);
        return *this;
    }

    unordered_multimap& operator=(std::initializer_list<value_type> ilist)
    {
        m_ht.clear();
        m_ht.reserve(ilist.size());
        for( auto first = ilist.begin(), last = ilist.end(); first != last; ++first )
            m_ht.insert_multi_noresize(*first);
        return *this;
    }

    ~unordered_multimap() = default;

    iterator begin() noexcept
    {
        return m_ht.begin();
    }

    iterator end() noexcept
    {
        return m_ht.end();
    }

    const_iterator begin() const noexcept
    {
        return m_ht.begin();
    }
    const_iterator end() const noexcept
    {
        return m_ht.end();
    }

    const_iterator cbegin() const noexcept
    {
        return m_ht.cbegin();
    }
    const_iterator cend() const noexcept
    {
        return m_ht.cend();
    }

    bool empty() const noexcept
    {
        return m_ht.empty();
    }

    size_t size() const noexcept
    {
        return m_ht.size();
    }

    size_t max_size() const noexcept
    {
        return m_ht.max_size();
    }

    template<class ...Args>
    iterator emplace(Args&& ...args)
    {
        return m_ht.emplace_multi(std::forward<Args>(args)...);
    }

    template<class ...Args>
    iterator emplace_hint(const_iterator hint, Args&& ...args)
    {
        return m_ht.emplace_multi_use_hint(hint, std::forward<Args>(args)...);
    }

    iterator insert(const value_type& value)
    {
        return m_ht.insert_multi(value);
    }

    iterator insert(value_type&& value)
    {
        return m_ht.emplace_multi(std::move(value));
    }


    iterator insert( const_iterator hint, const value_type& value)
    {
        return m_ht.insert_multi_use_hint(hint, value);
    }
    iterator insert( const_iterator hint, value_type&& value)
    {
        return m_ht.emplace_multi_use_hint(hint, std::move(value));
    }

    template<class T1>
    void insert(T1 first, T1 last)
    {
        m_ht.insert_multi(first, last);
    }

    void erase(iterator it)
    {
        m_ht.erase(it);
    }

    void erase(iterator first, iterator last)
    {
        m_ht.erase(first, last);
    }

    size_t erase( const key_type& key)
    {
        return m_ht.erase_multi(key);
    }

    void clear()
    {
        m_ht.clear();
    }

    void swap(unordered_multimap& other) noexcept
    {
        m_ht.swap(other.m_th);
    }


    size_t count( const key_type& key ) const
    {
        return m_ht.count(key);
    }

    iterator find(const key_type& key)
    {
        return m_ht.find(key);
    }

    const_iterator find( const key_type& key ) const
    {
        return m_ht.find(key);
    }

    std::pair<iterator, iterator> equal_range( const key_type& key)
    {
        return m_ht.equal_range_multi(key);
    }

    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
    {
        return m_ht.equal_range_multi(key);
    }

    local_iterator begin( size_t n ) noexcept
    {
        return m_ht.begin(n);
    }
    const_local_iterator begin(size_t n ) const noexcept
    {
        return m_ht.begin(n);
    }
    const_local_iterator cbegin(size_t n ) const noexcept
    {
        return m_ht.cbegin(n);
    }

    local_iterator end(size_t n ) noexcept
    {
        return m_ht.end(n);
    }
    const_local_iterator end( size_t n ) const noexcept
    {
        return m_ht.end(n);
    }
    const_local_iterator cend( size_t n ) const noexcept
    {
        return m_ht.cend(n);
    }

    size_t bucket_count() const noexcept
    {
        return m_ht.bucket_count();
    }
    size_t max_bucket_count() const noexcept
    {
        return m_ht.max_bucket_count();
    }

    size_t bucket_size(size_t n ) const noexcept
    {
        return m_ht.bucket_size(n);
    }
    size_t bucket( const key_type& key) const
    {
        return m_ht.bucket(key);
    }

    float load_factor() const noexcept
    {
        return m_ht.load_factor();
    }

    float max_load_factor() const noexcept
    {
        return m_ht.max_load_factor();
    }
    void max_load_factor( float mlf )
    {
        m_ht.max_load_factor(mlf);
    }

    void rehash( size_t count )
    {
        m_ht.rehash(count);
    }

    void reserve( size_t count )
    {
        m_ht.reserve(count);
    }

    hasher hash_fcn() const
    {
        return m_ht.hash_fcn();
    }

    key_equal key_eq() const
    {
        return m_ht.key_eq();
    }

  public:
    friend bool operator==(const unordered_multimap& lhs, const unordered_multimap& rhs )
    {
        return lhs.m_ht.equal_range_multi(rhs.m_ht);
    }

    friend bool operator!=( const unordered_multimap& lhs, const unordered_multimap& rhs )
    {
        return !lhs.m_ht.equal_range_multi(rhs.m_ht);
    }
};

template <class Key, class T, class Hash, class keyEqual>
bool operator==(const unordered_multimap<Key, T, Hash, keyEqual>& lhs,
                const unordered_multimap<Key, T, Hash, keyEqual>& rhs )
{
    return lhs == rhs;
}

template <class Key, class T, class Hash, class keyEqual>
bool operator!=(const unordered_multimap<Key, T, Hash, keyEqual>& lhs,
                const unordered_multimap<Key, T, Hash, keyEqual>& rhs )
{
    return lhs != rhs;
}

template <class Key, class T, class Hash, class keyEqual>
void swap(unordered_multimap<Key, T, Hash, keyEqual>& lhs,
          unordered_multimap<Key, T, Hash, keyEqual>& rhs)
{
    lhs.swap(rhs);
};

#endif
