#include <iostream>

using namespace std;

template<class T>
class A{
  public:
    void ps(T t)
    {
        cout << t << endl;
    }
};

template<>
class A<int>{
  public:
    void ps( int t )
    {
        cout << t << endl;
    }
};

int main()
{
    A<int> a;
    a.ps(3);

    return 0;
}