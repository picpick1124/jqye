#include <iostream>
#include <initializer_list>
#include <algorithm>

using namespace std;

template<class Object>
class Stack{
  private:
    struct Node{
        Object data;
        Node* next;
        Node(const Object& d = Object{}, Node* n = nullptr)
                :data{d}, next{n} {}

        Node(Object&& d, Node* n = nullptr)
                :data{std::move(d)}, next{n} {}
    };

  public:
    class const_iterator{
      public:
        const_iterator() :current(nullptr) {}

        Object& operator*() {return retrieve();}

        const_iterator& operator++()
        {
            current = current -> next;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator old = *this;
            ++(*this);
            return old;
        }

        bool operator==(const const_iterator& rhs) const
        { return current==rhs.current;}

        bool operator!=(const const_iterator& rhs) const
        { return !(*this==rhs);}

      protected:
        Node* current;

        const_iterator(Node* p) : current(p) {}
        Object& retrieve() const {return current -> data;}

        friend class Stack<Object>;

    };

    class iterator:public const_iterator{
        iterator(){}

        Object& operator*() {return const_iterator::retrieve();}
        const Object& operator*() const {return const_iterator::operator*();}

        iterator& operator++()
        {
            this -> current = this -> current -> next;
            return *this;
        }

        iterator operator++(int)
        {
            const_iterator old = *this;
            ++(*this);
            return old;
        }
      protected:

        iterator(Node* p) : const_iterator{p} {}

        friend class Stack<Object>;

    };


    Stack() { init();}

    Stack(const Stack& rhs)
    {
        init();
        for(auto x:rhs) {
            push(x);
        }
    }

    ~Stack()
    {
        clear();
        delete head;
        delete tail;
    }

    Stack(Stack&& rhs)
            :theSize(rhs.theSize), head(rhs.head), tail(rhs.tail)
    {
        rhs.theSize = 0;
        rhs.head = nullptr;
        rhs.tail = nullptr;
    }

    Stack& operator=(const Stack& rhs)
    {
        Stack copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    Stack& operator=(Stack&& rhs)
    {
        std::swap(theSize, rhs.theSize);
        std::swap(head, rhs,head);
        std::swap(tail, rhs.tail);

        return *this;
    }

    void push(Object& x)
    {
        insert(x);
    }

    void push(Object&& x)
    {
        insert(x);
    }

    void pop()
    {
        erase();
    }

    bool empty() {return theSize == 0;}

    void clear()
    {
        while(!empty()) {
            pop();
        }
    }


    iterator begin() {return iterator(head -> next);}
    const_iterator begin() const  {return const_iterator(head -> next);}

    iterator end() {return iterator(tail);}
    const_iterator end() const  {return const_iterator(tail);}

    iterator insert(Object& x)
    {
        Node* p = new Node{x, begin().current};
        p -> next = head -> next;
        head -> next = p;
        theSize++;
        return p;
    }

    iterator insert(Object&& x)
    {
        Node* p = new Node{std::move(x), begin()};
        p -> next = head -> next;
        head -> next = p;
        theSize++;
        return p;
    }
    iterator erase()
    {
        cout << *begin() << endl;
        head -> next = begin().current -> next;
        iterator retVal = iterator(head -> next);
        theSize--;
        return retVal;

    }

  private:
    int theSize;
    Node* head;
    Node* tail;

    void init()
    {
        theSize = 0;
        head = new Node;
        tail = new Node;
        head -> next = tail;
    }


};

int main()
{
    Stack<int> a;
    a.push(8);
    a.push(9);
    a.push(18);
    a.clear();
    return 0;
}
