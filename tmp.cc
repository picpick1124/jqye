#include <iostream>
#include <utility>
#include <type_traits>

using namespace std;

struct B{
    using first_type = int;
    using second_type = double;
    first_type a;
    second_type b;
};
template<class T>
class A{
  public:
    typedef typename std::remove_cv<typename T::first_type>::type key_type;
    key_type a;
    void ps()
    {
        cout << a << endl;
    }

};

int main()
{
    // pair<int, logn> b ;
    // A<pair<int, long>> a;
    // A<B> a;
    // a.ps();

    const size_t fnv_offset = 14695981039346656037ull;
    const size_t fnv_prime = 1099511628211ull;

    int count = 9932;
    int* pcount = new int(count);
    int result = 0;
    unsigned char *first = nullptr;
    first = (unsigned char*)&count;
    for (size_t i = 0; i < count; ++i) {
      result ^= (size_t)first[i];
      result *= fnv_prime;
    }

    cout << result << endl;
}