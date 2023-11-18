#include <iostream>
#include <initializer_list>
#include <vector>
#include <algorithm>

using namespace std;

template<class Object>
class Queue{
private:
    struct Node {
        Object data;
        Node* next;
        Node(cons Object& d = Object{}, Node n = nullptr)
            :data(d), next(n) { }
        Node(Object&& d, Node n = nullptr)
            :data(d), next(n) { }
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

        friend class Queue<Object>;

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

        friend class Queue<Object>;

    };


    Queue() { init();}

    Queue(const Queue& rhs)
    {
        init();
        for(auto x:rhs) {
            push(x);
        }
    }

    ~Queue()
    {
        clear();
        delete head;
        delete tail;
    }

    Queue(Queue&& rhs)
            :theSize(rhs.theSize), head(rhs.head), tail(rhs.tail)
    {
        rhs.theSize = 0;
        rhs.head = nullptr;
        rhs.tail = nullptr;
    }

    Queue& operator=(const Queue& rhs)
    {
        Queue copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    Queue& operator=(Queue&& rhs)
    {
        std::swap(theSize, rhs.theSize);
        std::swap(head, rhs,head);
        std::swap(tail, rhs.tail);

        return *this;
    }

    const_iterator begein() const
    {
        return {head -> next};
    }

    iterator begein()
    {
        return {head -> next};
    }


    const_iterator end() const
    {
        return {tail};
    }

    iterator end()
    {
        return {tail};
    }

    iterator inseart(const Object& x)
    {
        return {new Node(x,)}
    }

    iterator erase()
    {
        Node* p = end().current;
        head -> next = begin().next;
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
    return 0;
}
