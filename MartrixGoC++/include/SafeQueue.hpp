//
// Created by 11409 on 2021/5/15.
//

#ifndef MARTRIXGOC_SAFEQUEUE_HPP
#define MARTRIXGOC_SAFEQUEUE_HPP

#include <queue>
#include <mutex>

template <typename T>
class SafeQueue
{
private:
    std::queue<T> SQueue;
    std::mutex SMutex;

public:
    SafeQueue() = default;

    ~SafeQueue() = default;

    bool empty()
    {
        std::unique_lock<std::mutex> lock(SMutex);
        return SQueue.empty();
    }

    size_t size()
    {
        std::unique_lock<std::mutex> lock(SMutex);
        return SQueue.size();
    }

    void enqueue(T &t)
    {
        std::unique_lock<std::mutex> lock(SMutex);
        SQueue.push(t);
    }

    bool dequeue(T &t)
    {
        std::unique_lock<std::mutex> lock(SMutex);
        if(SQueue.empty())
        {
            return false;
        }
        t = std::move(SQueue.front());
        SQueue.pop();
        return true;
    }
};


#endif //MARTRIXGOC_SAFEQUEUE_HPP
