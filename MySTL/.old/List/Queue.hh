#include <iostream>
#include <initializer_list>
#include <vector>
#include <algorithm>

using namespace std;

#ifndef QUEUE_HH
#define QUEUE_HH
template<class Object>
class Queue{
private:
    struct Node {
        Object data;
        Node* next;
        Node(const Object& d = Object{}, Node n = nullptr)
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

    void clear()
    {
        theSize = 0;
        if( head == nullptr ) return;
        Node* tmp = head -> next;
        Node* ptmp;

        while( tmp ) {
            ptmp = tmp -> next;
            delete tmp;
            tmp = ptmp;
        }
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


// using namespace std;

// typedef int ElemType;

// struct LNode{
//     ElemType data;
//     LNode* next;
// };


// struct LinkQueue{
//     LNode* head;
//     LNode* tail;
// };


// // 对啊,既然是结构体,为什么不用引用而要用指针呢?
// LinkQueue* InitQueue()
// {
//     LNode* head = new( std::nothrow ) LNode;

//     if( head == nullptr ) return nullptr;

//     // 需要添加上这一句
//     head -> next = nullptr;

//     LinkQueue* init = new LinkQueue;

//     if( init == nullptr ) return nullptr;

//     init -> head = init -> tail = head;

//     return init;
// }


// bool PushQueue( LinkQueue* qu, const ElemType & ee )
// {
//     // LinkQueue是初始化分配的, 如果初始化不为空, 那么肯定为LinkQueueeue分配了内存
//     if( qu == nullptr ) return false;

//     LNode* tmp = new (std::nothrow) LNode;

//     if( tmp == nullptr ) return false;

//     tmp -> data = ee;
//     tmp -> next = nullptr;

//     // 维护链表
//     qu -> tail -> next = tmp;
//     qu -> tail = tmp;

//     return true;
// }

// bool PopQueue( LinkQueue* qu, ElemType & ee)
// {
//     if( qu == nullptr ) return false;
//     if(qu -> head -> next == nullptr ){
//         cout << "队列为空" << endl;
//         return false;
//     }

//     LNode* tmp = qu -> head -> next;

//     ee = tmp -> data;

//     if( qu -> head -> next == qu -> tail )
//         qu -> tail = qu -> head;

//     qu -> head -> next = tmp -> next;

//     delete tmp;

//     return true;
// }

// size_t QueueLength( LinkQueue* qu )
// {
//     if( qu == nullptr ) return 0;

//     LNode* tmp = qu -> head -> next;

//     size_t length = 0;

//     while( tmp ) {
//         ++length;
//         tmp = tmp -> next;
//     }

//     return length;
// }


// void PrintQueue( LinkQueue* qu )
// {
//     if( qu == nullptr ) return;

//     LNode* tmp = qu -> head -> next;

//     while( tmp ) {
//         cout << tmp -> data << ' ';
//         tmp = tmp -> next;
//     }

//     cout << endl;
// }

// bool DestroyQueue( LinkQueue* qu )
// {
//     if( qu == nullptr ) return false;

//     LNode* tmp = qu -> head;

//     while( qu -> head ) {
//         tmp = qu -> head -> next;
//         delete qu -> head;
//         qu -> head = tmp;
//     }

//     delete qu;
//     qu = nullptr;

//     return true;
// }

// void ClearQueue(LinkQueue * qu)
// {
//     if( qu == nullptr ) return;

//     LNode* tmp = qu -> head -> next;
//     LNode* ptmp;

//     // 不能直接用这个方法, 会改变实际指向head的指针
//     // while( tmp ) {
//     //     qu -> head = tmp -> next;
//     //     delete tmp;
//     //     tmp = qu -> head;
//     // }

//     while( tmp ) {
//         ptmp = tmp -> next;
//         delete tmp;
//         tmp = ptmp;
//     }
//     qu -> head -> next = nullptr;
//     qu -> tail = qu -> head;
// }

#endif