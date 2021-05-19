//
// Created by 11409 on 2021/5/15.
//

#ifndef MARTRIXGOC_THREADPOO_HPP
#define MARTRIXGOC_THREADPOO_HPP

#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <thread>
#include <atomic>
#include "fstream"

class ThreadPool
{
public:
    size_t poolSize;
    std::atomic<int> queueSize = 0;

    explicit ThreadPool(size_t);

    template<class F, class... Args>
    auto commit(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;

    template<class F, class... Args>
    void addTask(F&& f, Args&&... args);

    ~ThreadPool();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

inline ThreadPool::ThreadPool(size_t threads)
{
    this->poolSize = threads;
    this->stop = false;
    for(size_t i = 0; i < threads; i++)
    {
        //emplace_back 是异步操作，添加线程时，线程已经被启动了，主线程不会死循环
        workers.emplace_back(
                [this]
                {
                    for(;;)
                    {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->queueMutex);
                            // 获取到锁并且条件(p)为false时才会wait。并且一旦wait就会释放锁。
                            // 条件(p)：此处的条件(p)起的作用是：如果线程池的stop被置为了false并且任务队列为空时，即便接收到了notify消息，也继续wait
                            // 解释：线程池的stop标记一般为false表示线程池没有被关闭。
                            //      任务队列如果不为空并且此时接收到了唤醒信号(notify)就不wait了，直接开始执行任务。
                            //      如果任务队列为空，即便接到了唤醒信号(notify)那么就继续wait等候任务到来(此时由于是wait状态所以锁会被释放)
                            // 以上不wait的前提是，这个线程被唤醒(notify)了，唤醒的方式有可以是~ThreadPool中的notify_all
                            // 也可以是enqueue中的notify_one。总之是只有接收到唤醒信号，并且拿到锁并且(p)为true才会解除阻塞。
                            this->condition.wait(lock,[this]{return this->stop || !this->tasks.empty();});
                            // 如果stop为true并且任务队列空了，就需要结束死循环了并且返回了。
                            if(this->stop && this->tasks.empty())
                                return;
                            // 使用move可以减少拷贝次数，取出task并且脱离锁的作用域，释放锁，执行任务。
                            task = std::move(this->tasks.front());
                            // 任务被取出，任务队列pop
                            this->tasks.pop();
                            queueSize--;
                        }
                        if(task)
                            task();
                    }
                }
        );
    }
}

template<class F, class... Args>
auto ThreadPool::commit(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using returnType = typename std::result_of<F(Args...)>::type;

    // make_shared：定义智能指针，会自动回收task，只需要关注make_shared包装的内容即可。
    // packaged_task：包装一个可以被调用的对象，配合future使用可以异步获取该可调用对象产生的结果。
    // 此时被包装的函数无返回值。(也可以理解成被包装成了一个void函数)
    // bind：接受一个可调用对象，生成一个新的可调用对象来“适应”原对象的参数列表。此处的适应是指，可以把被调用的对象的一些参数固定下来。如下：
    //    double my_divide (double x, double y) {return x/y;}
    //    auto fn_half = std::bind (my_divide,_1,2); 此处，把第二个参数固定了下来为2，第一个参数仍需要传递。
    //    std::cout << fn_half(10) << '\n'; 被包装后的函数使用时只需要传入一个参数即可。
    // forward：完美转发不论传入的是任何左值或者右值都会按照原来的类型传参。
    auto task = std::make_shared< std::packaged_task<returnType()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    // 把返回值绑定future包装的res中，便可以异步获取res的结果。
    std::future<returnType> res = task->get_future();
    {
        // 加锁
        std::unique_lock<std::mutex> lock(queueMutex);

        // 如果线程池关闭了就不能再入队了。
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        // 把使用匿名函数作为可以被添加到任务队列的void函数加入队列。
        // [task]表示匿名函数的外部变量的列表。第一个()内放的是外部的形参列表。由于task已经被打包，所以不需要传递形参。
        // {}是匿名函数的函数体。(*task)取函数指针的内容。 第二个()表示直接执行task即可。
        // 返回值由于已经绑定到了之前定义的res。所以函数体内部不写return，这是一个void并且没有形参的函数。
        tasks.emplace([task]()
                      { (*task)(); });
    }
    // 随便唤醒一个线程。
//        markFileStream << "notify one start.\n";
//        markFileStream.flush();
//    clock_t start = std::clock();
    condition.notify_one();
//    clock_t end = std::clock();
//        markFileStream << "notify one end.\n";
//        markFileStream << "notify one time span:" << end - start << '\n';
//        markFileStream.flush();
    return res;
}

template<class F, class... Args>
void ThreadPool::addTask(F&& f, Args&&... args)
{
    std::unique_lock<std::mutex> lock(queueMutex);
    if(stop)
    {
        lock.unlock();
        f(args...);
    }
    else
    {
        tasks.push(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        queueSize++;
        condition.notify_one();
    }
}

inline ThreadPool::~ThreadPool()
{
    {
        //获取锁并且设置stop为true(关闭线程池)
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    // 通知所有的线程，不再wait。同时如果一个线程是wait状态，那么本来应该是释放了锁的，但是如果停止了wait那么就会重新获得这个锁。
    condition.notify_all();
    // 等待所有线程执行完。
    for(std::thread &worker: workers)
        worker.join();
}

#endif //MARTRIXGOC_THREADPOO_HPP
