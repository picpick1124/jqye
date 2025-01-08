#ifndef DEQUE_HH
#define DEQUE_HH

#include <initializer_list>
#include <memory>
#include <utility>

namespace mystd{

template<class T>
struct deque_default_size{
    static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
};

template<class T>
class deque{
    struct iterator;
    struct const_iterator;

  public:
    struct iterator {

    };
};


}

#endif
