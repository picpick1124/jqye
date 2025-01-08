#pragma once
#ifndef THREADPOOL_HH
#define THREADPOOL_HH

#include <memory>
#include <queue>
#include <deque>
#include <mutex>
#include <functional>
#include <future>
#include <vector>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <atomic>
#include <type_traits>
#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>



#ifndef _WIN32
    #define WINDOWS_MACRO
#else
    #ifdef threadpool_EXPORTS
        #define WINDOWS_MACRO __declspec(dllexport) //库项目调用
    #else
        #define WINDOWS_MACRO __declspec(dllimport) //调用库项目调用
    #endif
#endif

// 线程安全队列
template <class T>
class SafeQueue{
  private:
    std::queue<T, std::deque<T>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::atomic_bool m_stop;

  public:
    SafeQueue() : m_stop(false) {}
    ~SafeQueue()
    {
        stop();
    }

    bool empty()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    size_t size()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    // 入队操作, 接受左值和右值
    template<typename U>
    void push( U&& item)
    {
        {
            using U1 = typename std::remove_reference<U>::type;
            static_assert(std::is_same<U1, T>::value == true,
                          "SafeQueue element type is different from SafeQueue");
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.push(std::forward<U>(item));
        }
        m_condition.notify_one();
    }

    // 出队操作, 在此实现线程的阻塞和唤醒
    bool pop( T& item )
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [ this ](){
            return m_stop || !m_queue.empty();
        });

        if( m_stop && m_queue.empty() )
            return false;

        item = m_queue.front();
        m_queue.pop();
        return true;
    }


    // 唤醒所有线程, 停止入队操作
    void stop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if( false == m_stop ){
            m_stop = true;
            m_condition.notify_all();
        }
    }
};

class WINDOWS_MACRO ThreadPool{
  private:
    std::vector<std::thread*>  m_workers;
    // std::vector<std::thread>  m_workers;
    SafeQueue<std::function<void()>> m_tasks;
    std::condition_variable m_condition;
    std::mutex m_mutex;
    std::atomic_bool m_shutdown;

    // 关闭操作
    void shutdown()
    {
        // std::cout << "==================" << "m_tasks.size(): " << m_tasks.size() << "==============" << std::endl;

        // while( !m_tasks.empty()){
        // }
        // std::cout << "==================" << "m_tasks.size(): " << m_tasks.size() << "==============" << std::endl;

        m_shutdown = true;

        m_tasks.stop();

        // std::cout << "==================" << "m_tasks.size(): " << m_tasks.size() << "==============" << std::endl;

        int i = 0;

        for( std::thread* threads : m_workers){
            std::cout << threads << std::endl;
            std::cout <<" ==========================" << i <<" ========================= "<< std::endl;
            if( threads != nullptr && threads -> joinable()){
                threads -> join();
            }
            std::cout <<" ==========================" << i <<" ========================= "<< std::endl;
            i++;
        }


        i = 0;
        for( std::thread* threads : m_workers){
            std::cout << threads << std::endl;
            std::cout <<" ==========================" << i <<" ========================= "<< std::endl;
                delete threads;
                threads = nullptr;
            std::cout <<" ==========================" << i <<" ========================= "<< std::endl;
            i++;
        }


        std::cout << "Shutdown the ThreadPool ...\n";
    }
    /*
      你的代码逻辑大致上是正确的，但在 C++ 中，直接在循环中删除线程指针可能会导致问题。因为当你在循环中删除元素后，`threads` 容器的状态会改变，可能导致未定义行为或访问非法内存。

为了安全地释放资源，建议在等待线程完成后，单独进行内存释放。以下是修正后的示例：

```cpp
#include <iostream>
#include <thread>
#include <vector>

void threadFunction() {
    std::cout << "Thread is running\n";
}

int main() {
    std::vector<std::thread*> threads;

    // 创建线程并将其指针存储在 vector 中
    for (int i = 0; i < 5; ++i) {
        threads.push_back(new std::thread(threadFunction));
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        if (t->joinable()) {
            t->join(); // 加入线程
        }
    }

    // 释放资源
    for (auto& t : threads) {
        delete t; // 释放内存
    }

    return 0;
}
```

### 关键点

1. **分开 `join()` 和 `delete`**: 在一个循环中处理 `join()` 和 `delete` 可能会导致在 `delete` 后再访问已删除的指针。在这里，我们先等待所有线程完成，然后再单独释放内存。

2. **避免悬空指针**: 确保在调用 `delete` 之后不会再使用该指针。

这种方法可以避免潜在的错误和未定义行为，是管理线程资源的更安全方式。
     */


  public:

    // explicit ThreadPool( const int n  = std::thread::hardware_concurrency());

    explicit ThreadPool( const int num  = std::thread::hardware_concurrency() )
    : m_workers(num), m_shutdown(false)
    {
        for( int i = 0; i < num; i++ )
            m_workers[ i ] = new std::thread([this](){
                while(1){
                    {
                        std::function<void()> func;
                        if(!m_tasks.pop(func))
                            break;
                        // return;  // 这里也可以用return

                        if(func)
                            func(); // ps: 为了使func()有意义,需要有这样一个作用域,每次运行后销毁
                    }
                }
            });

        for( int i = 0; i < num; i++ ){
            std::cout << m_workers[i] << std::endl;
        }
        for( int i = 0; i < num; i++ ){
            std::cout << std::hex <<  (*m_workers[i]).native_handle() << std::endl;
        }
    }

    ~ThreadPool()
    {
        if( false == m_shutdown )
            shutdown();
    }


    ThreadPool(const ThreadPool& t ) = delete;
    ThreadPool( ThreadPool&& t ) = delete;
    ThreadPool &operator=(const ThreadPool& t ) = delete;
    ThreadPool &operator=(ThreadPool&& t ) = delete;


    // 提交任务函数,接受类普通成员函数,lambda表达式,普通函数等一切可调用对象
    template<typename Function, typename ... Args>
    auto submit( Function&& f, Args&& ... args) -> std::future<decltype(std::forward<Function>(f)(std::forward<Args>(args)...))>
    {
        std::function<decltype(std::forward<Function>(f)(std::forward<Args>(args)...))()> func =
                std::bind( std::forward<Function>(f), std::forward<Args>(args)...);
        // 连接函数和参数定义，特殊函数类型，避免左右值错误

        auto ptr = std::make_shared
                   <std::packaged_task<decltype(std::forward<Function>(f)(std::forward<Args>(args)...))()>>(func);

        std::function<void()> element = [ptr]() {
            (*ptr)();
        };

        m_tasks.push(element);
        // m_condition.notify_one();

        return ptr -> get_future();
    }
};

#endif
