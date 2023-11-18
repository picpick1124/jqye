#include <iostream>
#include <initializer_list>
#include <vector>
#include <algorithm>

using namespace std;

#ifndef STACK2
#define STACK2
template<class Object>
class Stack{
  public:
    Stack(vector<Object> d = vector<Object> {}, int t = -1)
            :data(d), topOfStack(t) {}

    void push(const Object& x)
    {
        ++topOfStack;
        data.resize(topOfStack+1);
        data[topOfStack] = x;
    }

    void push(Object&& x)
    {
        ++topOfStack;
        data.resize(topOfStack+1);
        data[topOfStack] = std::move(x);
    }

    void pop()
    {
        data[topOfStack];
        --topOfStack;
        data.pop_back();
    }

    void top()
    {
        // std::cout << data.back() << std::endl;
        std::cout << data[topOfStack] << std::endl;
    }

  private:
    vector<Object> data;
    int topOfStack {-1};
};

int main()
{
    Stack<int> a;
    int b  = 3;
    a.push(b);
    a.push(9);
    a.push(18);
    for(int i = 0; i < 3 ; ++i) {
        a.top();
        a.pop();
    }
    return 0;
}

#endif