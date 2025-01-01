#ifndef BLOCKQUEUE_HPP
#define BLOCKQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockQueue 
{
public:
    /// @brief 构造函数
    /// @param msize 阻塞队列最大容量
    BlockQueue(size_t msize = 1000) : maxSize(msize), isClose(false) {}

    void push(T&& item);
    bool pop(T& item);
    T front();
    T back();
    bool empty();
    bool full();
    void clear();
    void flush();
    void close();
    size_t getCurrSize();
    size_t getMaxSize();

private:
    std::queue<T> q;

    size_t maxSize;

    bool isClose;

    std::mutex mtx;
    std::condition_variable consumer;
    std::condition_variable producer;
};

template <typename T>
void BlockQueue<T>::push(T&& item)
{
    std::unique_lock<std::mutex> lock(mtx);
    while (q.size() >= maxSize && !isClose)
        producer.wait(lock);
    
    if (!isClose)
    {
        q.push(std::forward<T>(item));
        consumer.notify_one();
    }
}

template <typename T>
bool BlockQueue<T>::pop(T& item)
{
    std::unique_lock<std::mutex> lock(mtx);
    while (q.empty() && !isClose)
        consumer.wait(lock);
    
    if (!isClose)
    {
        item = q.front();
        q.pop();
        producer.notify_one();
    }
    return !isClose;
}

template <typename T>
T BlockQueue<T>::front()
{
    std::lock_guard<std::mutex> lock(mtx);
    return q.front();
}

template <typename T>
T BlockQueue<T>::back()
{
    std::lock_guard<std::mutex> lock(mtx);
    return q.back();
}

template <typename T>
bool BlockQueue<T>::empty()
{
    std::lock_guard<std::mutex> lock(mtx);
    return q.empty();
}

template <typename T>
bool BlockQueue<T>::full()
{
    std::lock_guard<std::mutex> lock(mtx);
    return q.size() >= maxSize;
}

template <typename T>
void BlockQueue<T>::clear()
{
    std::lock_guard<std::mutex> lock(mtx);
    while (!q.empty())
        q.pop();
}

template <typename T>
inline void BlockQueue<T>::flush()
{
    consumer.notify_one();
}

template <typename T>
inline void BlockQueue<T>::close()
{
    std::unique_lock<std::mutex> lock(mtx);
    isClose = true;
    lock.unlock();
    consumer.notify_all();
    producer.notify_all();
}

template <typename T>
size_t BlockQueue<T>::getCurrSize()
{
    std::lock_guard<std::mutex> lock(mtx);
    return q.size();
}

template <typename T>
size_t BlockQueue<T>::getMaxSize()
{
    return maxSize;
}

#endif