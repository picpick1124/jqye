#include <iostream>
#include <ostream>
#include <istream>
#include <initializer_list>
#include <algorithm>

using namespace std;
#ifndef D_CC
#define D_CC
template<class object>
class Vector {
  public:
    explicit Vector(int n = 0)
            :theSize(n), theCapacity(n + n?n:8)
    {
        p = new object[n + n?n:8];
    }

    ~Vector() {delete[] p;}

    Vector(const Vector& v)
            :theSize(v.theSize), theCapacity(v.theCapacity), p{nullptr}
    {
        p = new object[theCapacity];
        for(int i=0; i < theSize; ++i) {
            p[i] = v.p[i];
        }
    }

    Vector(Vector&& v)
            :theSize(v.theSize),theCapacity(v.theCapacity),p(v.p)
    {
        v.theSize = 0;
        v.theCapacity = 0;
        v.p = nullptr;
    }


    Vector& operator=(const Vector& v)
    {
        Vector copy = v;
        std::swap(*this, copy);
        // theSize = v.theSize;
        // theCapacity = v.theCapacity;
        // for(int i=0; i < v.theSize; ++i) {
        // p[i] = v.p[i];
        // }
        return *this;
    }

    Vector& operator=(Vector&& v)
    {
        // theSize = v.theSize;
        // theCapacity = v.theCapacity ;
        // p = v.p;
        std::swap(theSize, v.theSize);
        std::swap(theCapacity, v.theCapacity);
        std::swap(p, v.p);
        return *this;
    }


    Vector(initializer_list<object> lst) {
        theSize = lst.size();
        object* p = new object(2*theSize);
        auto itr = lst.begin();
        int i = 0;

        while(itr != lst.end()) {
            p[i] = *itr;
            ++i; ++itr;
        }
    }


    int size() const {return theSize;}

    bool empty(int n) const
    {
        return size() == 0;
    }

    void clear()
    {
        theSize = 0;
        theCapacity = 0;
        delete[] p;
        p = nullptr;
    }

    void resize(int newNumber)
    {
        if(newNumber > theCapacity) reserve(newNumber*2);
        theSize = newNumber;
    }


    void reserve(int n)
    {
        if(n < theCapacity) return;

        object* p1 = new object[n];
        for(int i=0; i < theSize; ++i) {
            p1[i] = std::move(p[i]);
        }
        theCapacity = n;
        std::swap(p, p1);
        delete[] p1;
    }

    void push_back(const object& x)
    {
        if(theSize < theCapacity) {
            p[theSize] = x;
        }
        else {
            reserve(2*theCapacity+1);
            p[theSize] = x;
        }
        theSize += 1;
    }
    void pop_back() { --theSize; }


    const object& back() const {return p[theSize-1];}
    object& back() {return p[theSize-1];}

    const object& front() const { return p[0];}
    object& front() { return p[0];}


    const object& operator[](int n) const
    {
        return p[n];
    }

    object& operator[](int n)
    {
        return p[n];
    }

    object& at(int n)
    {
        if(n>=theSize) {
            cout << "N is too large" << endl;
        }
        else {
            return p[n];
        }
    }

    const object& at(int n) const
    {
        if(n>=theSize) {
            cout << "N is too large" << endl;
        }
        else {
            return p[n];
        }
    }

    int ca() const { return theCapacity;}

    typedef object* iterator;
    typedef const object* const_iterator;

    iterator begin()
    {
        return &p[0];
    }
    iterator begin() const
    {
        return &p[0];
    }

    const_iterator end()
    {
        return &p[theSize];
    }

    const_iterator end() const
    {
        return &p[theSize];
    }
    // void pf()
    // {
    //     for(int i = 0; i < theSize; ++i) {
    //         cout << p[i] << endl;
    //     }
    // }

  private:
    int theSize;
    int theCapacity ;
    object* p;
};

#endif

int main()
{

    Vector<int> Vec;
    Vec[3] = 99;
    Vec.push_back(88);
    Vector<int> Vec1;
    Vec1 = Vec;
    cout << "--------" << endl;
    cout << Vec[0] << endl;
    cout << Vec1[0] << endl;

    cout << "--------" << endl;
    cout << Vec.size() << endl;
    cout << Vec1.size() << endl;
    return 0;
}
