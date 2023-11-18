#include <iostream>
#include <initializer_list>
#include <vector>
#include <algorithm>

using namespace std;

#ifndef STACK1
#define STACK1
template<class Object>
class Stack{
  public:
    void push(Object& x)
    {
        data.push_back(x);
    }

    void push(Object&& x)
    {
        data.push_back(std::move(x));
    }

    void pop()
    {
        data.pop_back();
    }

    void top()
    {
        std::cout << data.back() << std::endl;
    }
  private:
    vector<Object> data;
};

int main()
{
    Stack<int> a;
    a.push(8);
    a.push(9);
    a.push(18);
    for(int i = 0; i < 3 ; ++i) {
        a.top();
        a.pop();
    }
    return 0;
}





#endif